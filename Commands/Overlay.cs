using ManyConsole;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using ELFSharp.ELF;
using ELFSharp.Utilities;
using ELFSharp.ELF.Segments;
using ELFSharp.ELF.Sections;
using MiscUtil.IO;
using MiscUtil.Conversion;
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

        public bool ZZRomTool { get; set; }

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

            HasOption<bool>("zzromtool:", "Convert the overlay to a form compatible with zzromtool", value => ZZRomTool = true);

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
            using (var elf = ELFReader.Load<uint>(InputFiles[0]))
            {
                var textSection = elf.Sections.Where(section => ".text".Equals(section.Name)).FirstOrDefault();
                var dataSection = elf.Sections.Where(section => ".data".Equals(section.Name)).FirstOrDefault();
                var rodataSection = elf.Sections.Where(section => ".rodata".Equals(section.Name)).FirstOrDefault();
                var bssSection = elf.Sections.Where(section => ".bss".Equals(section.Name)).FirstOrDefault();

                if (textSection == null)
                {
                    throw new KeyNotFoundException("Could not find .text section in elf file!  Overlay invalid without code.");
                }

                if (dataSection == null && rodataSection == null)
                {
                    throw new KeyNotFoundException("No .data or .rodata sections found in elf file!  Overlay invalid without " + InitializationVariable + ".");
                }

                var programSection = elf.Segments.Where(segment => segment.Flags.HasFlag(SegmentFlags.Execute)).FirstOrDefault();

                if (programSection == null && !ForceNoProgram)
                {
                    throw new KeyNotFoundException("Could not find executable program section in the overlay.");
                }

                var symbolTable = elf.GetSections<SymbolTable<uint>>().FirstOrDefault();
                var relocs = GenerateRelocationSection(elf, symbolTable);
                SymbolEntry<uint> initSymbol = null;

                if (symbolTable == null && ShowInitAddress)
                {
                    throw new KeyNotFoundException("Could not find symbol table in the overlay.");
                }
                else
                {
                    initSymbol = symbolTable.Entries.Where(symbol => InitializationVariable.Equals(symbol.Name)).FirstOrDefault();
                }

                int length = 0;

                if (initSymbol == null && ShowInitAddress)
                {
                    throw new KeyNotFoundException("Could not locate initialization structure with symbol name " + InitializationVariable + ".");
                }

                if (initSymbol == null && ZZRomTool)
                {
                    throw new KeyNotFoundException("Could not locate initialization structure [" + InitializationVariable + "]: required for zzromtool compatibility mode.");
                }

                using (var overlayStream = new EndianBinaryWriter(EndianBitConverter.Big, File.Create(OverlayPath, 1024, FileOptions.RandomAccess)))
                {
                    overlayStream.Write(programSection.GetFileContents());

                    // Ensure the stream is pointing to the end of a 16-byte (64-bit) alignment
                    ForceAlignWrite(overlayStream, 16);

                    // Writing overlay header section
                    int position = (int)overlayStream.BaseStream.Position;
                    overlayStream.Write(textSection != null ? textSection.Size : 0);
                    overlayStream.Write(dataSection != null ? dataSection.Size : 0);
                    overlayStream.Write(rodataSection != null ? rodataSection.Size : 0);
                    overlayStream.Write(bssSection != null ? bssSection.Size : 0);

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

                    // If initialization symbol is known and zzromtool compatible mode is set, modify the initialization structure to include the 0xDEADBEEF constant.
                    // If there was a way to get around this..... I'd do it.
                    if (ZZRomTool && initSymbol != null)
                    {
                        var symbolPosition = initSymbol.Value - programSection.Address;
                        overlayStream.Seek((int) symbolPosition, SeekOrigin.Begin);
                        overlayStream.Write((ushort) 0xDEAD);
                        overlayStream.Seek(8, SeekOrigin.Current);
                        overlayStream.Write((ushort) 0xBEEF);
                    }
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
        }

        private static void ForceAlignWrite(EndianBinaryWriter overlayStream, int alignToByte)
        {
            if (overlayStream.BaseStream.Position % 16 != alignToByte % 16)
            {
                byte[] padding = new byte[alignToByte - (overlayStream.BaseStream.Position % 16)];
                overlayStream.Write(padding);
            }
        }

        private List<ZeldaReloc> GenerateRelocationSection(ELF<uint> elf, SymbolTable<uint> symbols)
        {
            var relocationSections = elf.Sections.Where(section => section.Type == SectionType.Relocation);

            var relocs = new List<ZeldaReloc>();

            foreach (var section in relocationSections)
            {
                var pointedSectionName = section.Name.Remove(0, 4);
                var pointedSection = elf.GetSection(pointedSectionName);

                var relocSection = new SimpleRelocationSection(section, symbols);

                foreach (var reloc in relocSection.Relocations)
                {
                    var symbol = reloc.Symbol;

                    // Skip symbols that are not defined in the object file.
                    if (".reginfo".Equals(symbol.PointedSection.Name))
                    {
                        continue;
                    }

                    var convertedReloc = ZeldaReloc.FromReloc(reloc, section, pointedSection.LoadAddress);
                    if (convertedReloc.HasValue)
                    {
                        relocs.Add(convertedReloc.Value);
                    }
                }
            }

            return relocs;
        }

        struct ZeldaReloc
        {
            public Section Section;

            public RelocationType Type;

            public uint Offset;

            public static ZeldaReloc? FromReloc(ICommonRelocation reloc, ISection parent, uint baseAddress)
            {
                var converted = new ZeldaReloc(parent, reloc, baseAddress);

                if (converted.Section == Section.UNKNOWN)
                {
                    return null;
                }
                else
                {
                    return converted;
                }
            }

            private ZeldaReloc(ISection section, ICommonRelocation reloc, uint baseAddress)
            {
                var ownedSectionName = section.Name;

                Section = SectionMethods.ConvertFromName(ownedSectionName);
                Type = reloc.Type;
                Offset = reloc.Offset - baseAddress;
            }

            public void Write(EndianBinaryWriter writer)
            {
                int reloc = ((byte)Section << 30) | ((byte)Type << 24) | (int)Offset;
                writer.Write(reloc);
            }
        }

        public enum Section : byte
        {
            UNKNOWN = 0,
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
                    case ".rel.text":
                        return Section.TEXT;
                    case ".rel.data":
                        return Section.DATA;
                    case ".rel.rodata":
                        return Section.RODATA;
                    case ".rel.bss":
                        return Section.BSS;
                    default:
                        return 0;
                }
            }
        }
    }
}
