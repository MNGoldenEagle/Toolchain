using System.Threading.Tasks;
using System.IO;
using MiscUtil.IO;
using MiscUtil.Conversion;
using ManyConsole;
using System;
using ELFSharp.ELF;

namespace Toolchain
{
    public class Viewer : ConsoleCommand
    {
        public string Filename { get; set; }

        public Viewer()
        {
            IsCommand("view", "View the structure of a .o file");

            HasLongDescription("Displays the complete structure for a compiled C file.  This command assumes that the compilation unit is in the ELF format.");

            HasAdditionalArguments(1, "The path to the .o file to view");

            SkipsCommandSummaryBeforeRunning();
        }

        public override int? OverrideAfterHandlingArgumentsBeforeRun(string[] remainingArguments)
        {
            if (remainingArguments.Length < 1)
            {
                throw new ConsoleHelpAsException("Filename not specified.");
            }

            Filename = remainingArguments[0];

            return base.OverrideAfterHandlingArgumentsBeforeRun(remainingArguments);
        }

        public override int Run(string[] remainingArguments)
        {
            try
            {
                ViewStructure();

                return 0;
            }
            catch (Exception e)
            {
                Console.Error.WriteLine(e);

                return 1;
            }
        }

        public void ViewStructure()
        {
            var elf = ELFReader.Load<uint>(Filename);

            Console.WriteLine(elf);
        }
    }
}
