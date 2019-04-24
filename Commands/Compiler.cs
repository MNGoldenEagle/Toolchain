using System;
using System.Diagnostics;
using System.Text;
using System.IO;
using ManyConsole;

namespace Toolchain
{
    public class Compiler : ConsoleCommand
    {
        public string[] SourceFiles { get; set; }

        public int OptimizeLevel { get; set; }

        public Compiler()
        {
            // Default to no optimization
            OptimizeLevel = 0;

            IsCommand("compile", "Compiles source files into a MIPS ELF object file.");

            HasLongDescription("Invokes the clang C cross-compiler to build MIPS assembly files based on the source files provided.");

            HasOption<int>("O|optimize=", "Specifies the optimization level (default is none)", value => OptimizeLevel = value);

            AllowsAnyAdditionalArguments("<C source files>");

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
            Pipeline.CompiledArtifacts = new string[SourceFiles.Length];
            Pipeline.Release = OptimizeLevel > 0;

            return Compile();
        }

        public int Compile()
        {
            var i = 0;

            foreach (var file in SourceFiles)
            {
                var outputFile = Path.GetFileNameWithoutExtension(file) + ".s";
                var args = new StringBuilder("-fno-addrsig -DF3DEX_GBI_2 -D_LANGUAGE_C -v -target mips-mips2-elf -march=mips2 -mabi=o32 -mfp64 -fomit-frame-pointer -S ");
                if (OptimizeLevel > 0)
                {
                    args.Append("-DRELEASE ");
                }
                else
                {
                    args.Append("-DDEBUG ");
                }
                args.AppendFormat("-I \"{0}\" ", Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Headers"));
                args.AppendFormat("-O{0} ", OptimizeLevel);
                args.AppendFormat("-o \"{0}\" ", outputFile);
                args.Append(file);

                Console.WriteLine("Compiling {0}...", file);

                ProcessStartInfo info = new ProcessStartInfo()
                {
                    FileName = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Binaries", "clang.exe"),
                    Arguments = args.ToString(),
                    CreateNoWindow = true,
                    ErrorDialog = false,
                    RedirectStandardInput = true,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true,
                    UseShellExecute = false,
                    WorkingDirectory = Directory.GetCurrentDirectory()
                };

                Process compiler = new Process
                {
                    StartInfo = info
                };

                compiler.OutputDataReceived += (sender, evt) => Console.Out.WriteLine(evt.Data);
                compiler.ErrorDataReceived += (sender, evt) => Console.Error.WriteLine(evt.Data);

                compiler.Start();
                compiler.BeginOutputReadLine();
                compiler.BeginErrorReadLine();

                while (!compiler.HasExited)
                {
                    compiler.WaitForExit(500);
                }

                if (compiler.ExitCode > 0)
                {
                    Console.Error.WriteLine("Compile failed for {0} (code was {1}), exiting.", file, compiler.ExitCode);
                    return compiler.ExitCode;
                }

                Pipeline.CompiledArtifacts[i] = outputFile;

                Console.WriteLine("Generated compilation unit: {0}", Pipeline.CompiledArtifacts[i++]);
            }

            return 0;
        }
    }
}
