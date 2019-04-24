using ManyConsole;
using System;
using System.Diagnostics;
using System.IO;
using System.Text;
using Commands;

namespace Toolchain
{
    public class Linker : ConsoleCommand
    {
        public string[] SourceFiles { get; set; }

        public string OutputFile { get; set; }

        public Target LinkTarget { get; set; }

        public Linker()
        {
            IsCommand("link", "Links one or more ELF object files into a single object file against the game library.");

            HasLongDescription("Invokes the GNU MIPS linker to build a linked ELF object file consisting of one or more unlinked files that are linked against the game's library.");

            HasRequiredOption<string>("o|output=", "Specifies the output filename", value => OutputFile = value);

            HasOption<string>("t|target=", "Specifies the target ROM type (e.g., 1.0U, 1.1U, 1.2E, Debug)", value => {
                if (string.IsNullOrWhiteSpace(value))
                {
                    LinkTarget = Target.PAL_MQ_DEBUG;
                }
                else
                {
                    LinkTarget = Target.FromValue(value.ToUpperInvariant());
                }
            });

            AllowsAnyAdditionalArguments("<files to link>");

            SkipsCommandSummaryBeforeRunning();
        }

        public override int? OverrideAfterHandlingArgumentsBeforeRun(string[] remainingArguments)
        {
            if (remainingArguments.Length < 1)
            {
                throw new ConsoleHelpAsException("No source files specified.");
            }

            if (LinkTarget == null)
            {
                LinkTarget = Target.PAL_MQ_DEBUG;
            }

            SourceFiles = remainingArguments;

            return base.OverrideAfterHandlingArgumentsBeforeRun(remainingArguments);
        }

        public override int Run(string[] remainingArguments)
        {
            return LinkObjectFiles();
        }

        public int LinkObjectFiles()
        {
            var args = new StringBuilder("-A elf32-bigmips --emit-relocs -S");
            string templateFilename = LinkTarget.DisplayName;

            args.AppendFormat(" -T \"{0}\"", Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Templates", templateFilename));
            args.AppendFormat(" -o \"{0}\"", OutputFile);

            if (Pipeline.Release)
            {
                args.Append(" -O1");
            }

            foreach (var file in SourceFiles)
            {
                args.AppendFormat(" \"{0}\"", file);
            }

            Console.WriteLine("Generating linked object file...");

            ProcessStartInfo info = new ProcessStartInfo()
            {
                FileName = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Binaries", "ld.exe"),
                Arguments = args.ToString(),
                CreateNoWindow = true,
                ErrorDialog = false,
                RedirectStandardInput = false,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
                WorkingDirectory = Directory.GetCurrentDirectory()
            };

            var linker = new Process
            {
                StartInfo = info
            };

            linker.OutputDataReceived += (sender, evt) => Console.Out.WriteLine(evt.Data);
            linker.ErrorDataReceived += (sender, evt) => Console.Error.WriteLine(evt.Data);

            linker.Start();
            linker.BeginOutputReadLine();
            linker.BeginErrorReadLine();

            while (!linker.HasExited)
            {
                linker.WaitForExit(500);
            }

            if (linker.ExitCode > 0)
            {
                Console.Error.WriteLine("Linker failed, exiting.");
                return linker.ExitCode;
            }

            Pipeline.LinkedArtifact = OutputFile;

            Console.WriteLine("Generated linked ELF object file: {0}", OutputFile);

            return 0;
        }
    }
}
