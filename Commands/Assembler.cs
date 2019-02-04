using ManyConsole;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Toolchain
{
    public class Assembler : ConsoleCommand
    {
        public string[] SourceFiles { get; set; }

        public string OutputFile { get; set; }

        public Assembler()
        {
            IsCommand("assemble", "Assembles an ELF object file from the raw assembly file (.S).");

            HasLongDescription("Invokes the GNU MIPS assembler to build the MIPS ELF object file (unlinked) based on a raw assembly file generated from a compiler or manually.");

            HasRequiredOption<string>("o|output=", "Specifies the output filename", value => OutputFile = value);

            AllowsAnyAdditionalArguments("Source files to assemble");

            SkipsCommandSummaryBeforeRunning();
        }

        public override int? OverrideAfterHandlingArgumentsBeforeRun(string[] remainingArguments)
        {
            if (remainingArguments.Length < 1)
            {
                throw new ConsoleHelpAsException("No source files specified.");
            }

            SourceFiles = remainingArguments;

            return base.OverrideAfterHandlingArgumentsBeforeRun(remainingArguments);
        }

        public override int Run(string[] remainingArguments)
        {
            return Assemble();
        }

        public int Assemble()
        {
            var args = new StringBuilder("-march mips3 -mabi 32 -mno-shared -call_nonpic -EB ");
            args.AppendFormat("-o {0} ", OutputFile);

            args.Append(SourceFiles.Aggregate((allFiles, filename) => allFiles += ' ' + filename));

            Console.WriteLine("Assembling to {0}...", OutputFile);

            var info = new ProcessStartInfo()
            {
                FileName = "as.exe",
                Arguments = args.ToString(),
                CreateNoWindow = true,
                ErrorDialog = false,
                RedirectStandardInput = true,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
                WorkingDirectory = Directory.GetCurrentDirectory()
            };

            var assembler = new Process
            {
                StartInfo = info
            };

            assembler.OutputDataReceived += (source, evt) =>
            {
                Console.Out.WriteLine(evt.Data);
            };
            assembler.ErrorDataReceived += (source, evt) =>
            {
                Console.Error.WriteLine(evt.Data);
            };

            assembler.EnableRaisingEvents = true;
            assembler.Start();
            assembler.BeginOutputReadLine();
            assembler.BeginErrorReadLine();

            while (!assembler.HasExited)
            {
                assembler.WaitForExit(50);
            }

            if (assembler.ExitCode != 0)
            {
                Console.Error.WriteLine("Failed to assemble {0} (exit code was {1}), exiting.", OutputFile, assembler.ExitCode);
                return assembler.ExitCode;
            }

            Pipeline.AssembledArtifact = OutputFile;

            Console.WriteLine("Assembled ELF object file: {0}", OutputFile);

            return 0;
        }
    }
}
