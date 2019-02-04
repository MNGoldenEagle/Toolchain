using ManyConsole;
using MiscUtil.Conversion;
using MiscUtil.IO;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using ELF;

namespace Toolchain
{
    public class Overlay : ConsoleCommand
    {
        public string[] InputFiles { get; set; }

        public string OverlayPath { get; set; }

        public bool ForceNoProgram { get; set; }

        public bool ShowInitAddress { get; set; }

        public bool Porcelain { get; set; }

        public string InitializationVariable { get; set; }

        public Overlay()
        {
            IsCommand("overlay", "Generates an overlay file");

            HasLongDescription("Generates an overlay file that is compatible with Zelda 64 ROMs.  The overlay requires ELF compilation units that contain relocation entries in order to properly generate the overlay.  This happens by default if you pair this option with the \"compile\" option.  The C linker must be in the PATH variable in order for this option to function properly.");

            HasRequiredOption<string>("o|output=", "The path to write the overlay to", value => OverlayPath = value);

            HasOption<string>("init:", "The name of the actor overlay's initialization variable (default is INIT)", value => InitializationVariable = value);

            HasOption<bool>("fno-program:", "Forces creation of an overlay even if no executable code is found", value => ForceNoProgram = true);

            HasOption<bool>("show-init-addr:", "Outputs the virtual address of the actor init structure (if true, init structure symbol must exist)", value => ShowInitAddress = true);

            HasOption<bool>("porcelain:", "Show all output in a format amenable to parsing", value => Porcelain = true);

            HasAdditionalArguments(1, "<input compilation unit>");
        }

        public override int? OverrideAfterHandlingArgumentsBeforeRun(string[] remainingArguments)
        {
            if (remainingArguments.Length > 0)
            {
                InputFiles = remainingArguments;
            }
            else if (Pipeline.CompiledArtifacts.Length > 0)
            {
                InputFiles = Pipeline.CompiledArtifacts;
            }
            else
            {
                throw new ConsoleHelpAsException("No input files specified.");
            }

            if (string.IsNullOrEmpty(InitializationVariable))
            {
                InitializationVariable = "INIT";
            }

            return base.OverrideAfterHandlingArgumentsBeforeRun(remainingArguments);
        }

        public override int Run(string[] remainingArguments)
        {
            string output = OverlayPath;

            if (string.IsNullOrEmpty(output))
            {
                var firstInputFile = new FileInfo(InputFiles[0]);
                output = firstInputFile.Name.Substring(0, firstInputFile.Name.IndexOf(firstInputFile.Extension));
            }

            GenerateOverlayFile(output);

            return 0;
        }

        private void GenerateOverlayFile(string objectFile)
        {
            Header elf;

            using (var file = File.Open(InputFiles[0], FileMode.Open, FileAccess.Read, FileShare.Read | FileShare.Delete))
            {
                using (var reader = new EndianBinaryReader(EndianBitConverter.Big, file))
                {
                    elf = new Header(reader);
                }
            }

            var relocs = GenerateRelocationSection(elf);

            var textSection = elf.SectionHeaders.Where(header => ".text".Equals(header?.GetName())).FirstOrDefault();
            var dataSection = elf.SectionHeaders.Where(header => ".data".Equals(header?.GetName())).FirstOrDefault();
            var rodataSection = elf.SectionHeaders.Where(header => ".rodata".Equals(header?.GetName())).FirstOrDefault();
            var bssSection = elf.SectionHeaders.Where(header => ".bss".Equals(header?.GetName())).FirstOrDefault();

            if (textSection == null)
            {
                throw new KeyNotFoundException("Could not find .text section in elf file!  Overlay invalid without code.");
            }

            if (dataSection == null && rodataSection == null)
            {
                throw new KeyNotFoundException("No .data or .rodata sections found in elf file!  Overlay invalid without " + InitializationVariable + ".");
            }

            var programSection = elf.ProgramHeaders.Where(header => header.Flags.HasFlag(SegmentFlags.EXECUTABLE)).FirstOrDefault();

            if (programSection == null && !ForceNoProgram)
            {
                throw new KeyNotFoundException("Could not find executable program section in the overlay.");
            }

            var initSymbol = (elf.SectionHeaders.Where(header => header.Type == SectionType.SYM_TABLE).First()?.SectionData as SymbolTable)?.Symbols.Where(symbol =>
                InitializationVariable.Equals(symbol?.GetName())).FirstOrDefault();
            int length = 0;

            if (initSymbol == null && ShowInitAddress)
            {
                throw new KeyNotFoundException("Could not locate initialization structure with symbol name " + InitializationVariable + ".");
            }

            using (var overlayStream = new EndianBinaryWriter(EndianBitConverter.Big, File.Create(OverlayPath, 1024, FileOptions.RandomAccess)))
            {
                overlayStream.Write(programSection.Data);

                // Ensure the stream is pointing to the end of a 16-byte (64-bit) alignment
                ForceAlignWrite(overlayStream, 16);

                // Writing overlay header section
                int position = (int)overlayStream.BaseStream.Position;
                overlayStream.Write(textSection != null ? textSection.SegmentImageSize : 0);
                overlayStream.Write(dataSection != null ? dataSection.SegmentImageSize : 0);
                overlayStream.Write(rodataSection != null ? rodataSection.SegmentImageSize : 0);
                overlayStream.Write(bssSection != null ? bssSection.SegmentImageSize : 0);

                // Write relocation section
                overlayStream.Write(relocs.Count);

                foreach (var reloc in relocs)
                {
                    reloc.Write(overlayStream);
                }

                // Ensure the stream is pointing to the last word in a 64-bit alignment
                ForceAlignWrite(overlayStream, 12);

                length = (int)overlayStream.BaseStream.Position + 4;
                var reverseOffset = length - position;
                overlayStream.Write(reverseOffset);
            }

            if (!Porcelain)
            {
                Console.WriteLine("Overlay written to {0}.", OverlayPath);
            }
            else
            {
                Console.WriteLine(OverlayPath);
            }

            if (initSymbol != null && ShowInitAddress)
            {
                if (!Porcelain)
                {
                    Console.WriteLine("{0} structure located at {1:X}.", InitializationVariable, initSymbol.Value);
                }
                else
                {
                    Console.WriteLine("{0:X}", initSymbol.Value);
                }
            }
        }

        private static void ForceAlignWrite(EndianBinaryWriter overlayStream, int alignToByte)
        {
            if (overlayStream.BaseStream.Position % 16 != alignToByte % 16)
            {
                byte[] padding = new byte[alignToByte - (overlayStream.BaseStream.Position % 16)];
                overlayStream.Write(padding);
            }
        }

        private List<ZeldaReloc> GenerateRelocationSection(Header header)
        {
            var relocationSections = header.SectionHeaders.Where(currentHeader => currentHeader.Type == SectionType.RELOC || currentHeader.Type == SectionType.RELOC_ADD);

            var relocs = new List<ZeldaReloc>();

            foreach (var section in relocationSections)
            {
                var symbolTable = (SymbolTable)header.SectionHeaders[section.LinkIndex].SectionData;

                if (section.Type == SectionType.RELOC)
                {
                    var relocSection = ((SimpleRelocations)section.SectionData);

                    foreach (var reloc in relocSection.Relocations)
                    {
                        var symbol = symbolTable.Symbols[reloc.SymbolTargetIndex];

                        if (symbol.BindType != BindingType.LOCAL)
                        {
                            continue;
                        }

                        relocs.Add(new ZeldaReloc(header, section, reloc));
                    }
                }
                else if (section.Type == SectionType.RELOC_ADD)
                {
                    var relocSection = ((AddendRelocations)section.SectionData);

                    foreach (var reloc in relocSection.Relocations)
                    {
                        var symbol = symbolTable.Symbols[reloc.SymbolTargetIndex];

                        if (symbol.BindType != BindingType.LOCAL)
                        {
                            continue;
                        }

                        relocs.Add(new ZeldaReloc(header, section, reloc));
                    }
                }
            }

            return relocs;
        }

        struct ZeldaReloc
        {
            public Section Section;

            public RelocationType Type;

            public int Offset;

            public ZeldaReloc(Header header, SectionHeader section, ICommonRelocation reloc)
            {
                var ownedSectionName = header.SectionHeaders[section.Info].GetName();

                Section = SectionMethods.ConvertFromName(ownedSectionName);
                Type = reloc.Type;
                Offset = reloc.Offset;
            }

            public void Write(EndianBinaryWriter writer)
            {
                int reloc = ((byte)Section << 30) | ((byte)Type << 24) | Offset;
                writer.Write(reloc);
            }
        }

        public enum Section : byte
        {
            TEXT = 1,
            DATA = 2,
            RODATA = 3,
            BSS = 4
        }

        public static class SectionMethods
        {
            public static Section ConvertFromName(string name)
            {
                switch (name)
                {
                    case ".text":
                        return Section.TEXT;
                    case ".data":
                        return Section.DATA;
                    case ".rodata":
                        return Section.RODATA;
                    case ".bss":
                        return Section.BSS;
                    default:
                        return 0;
                }
            }
        }
    }
}
