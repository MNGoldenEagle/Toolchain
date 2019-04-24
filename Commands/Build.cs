using ManyConsole;
using System;
using System.IO;

namespace Toolchain
{
    public class Build : ConsoleCommand
    {
        public string[] SourceFiles { get; set; }

        public string OutputFile { get; set; }

        public Target Target { get; set; }

        public bool Release { get; set; }

        public Build()
        {
            IsCommand("build", "Builds a linked, injectable overlay from the provided source files.");

            HasLongDescription("Generates an overlay based on the provided source files by invoking the compile, assemble, link, and overlay steps consecutively using default options.");

            HasRequiredOption<string>("o|output=", "Specifies the name of the generated actor file", value => OutputFile = value);

            HasOption<string>("t|target:", "Indicates which version of the ROM to link against.", value => {
                var target = Target.PAL_MQ_DEBUG;
                if (!Target.TryParse(value, out target))
                {
                    target = Target.PAL_MQ_DEBUG;
                }
                Target = target;
            });

            HasOption<bool>("release", "Indicates that the overlay is in a release-ready state. This will apply more aggressive optimizations and define the RELEASE macro.",
                value => Release = value);

            AllowsAnyAdditionalArguments("<C source files>");

            SkipsCommandSummaryBeforeRunning();
        }

        public override int? OverrideAfterHandlingArgumentsBeforeRun(string[] remainingArguments)
        {
            if (remainingArguments.Length < 1)
            {
                throw new ConsoleHelpAsException("No source files specified.");
            }

            if (Target == null)
            {
                Target = Target.PAL_MQ_DEBUG;
            }

            SourceFiles = remainingArguments;

            return base.OverrideAfterHandlingArgumentsBeforeRun(remainingArguments);
        }

        public override int Run(string[] remainingArguments)
        {
            Compiler compileStep = new Compiler
            {
                SourceFiles = SourceFiles,
                OptimizeLevel = Release ? 4 : 0
            };
            int resultCode = compileStep.Run(null);
            if (resultCode != 0)
            {
                Console.Error.WriteLine("Error occurred on Compile step.  Exiting...");
                return resultCode;
            }

            Assembler assembleStep = new Assembler
            {
                SourceFiles = Pipeline.CompiledArtifacts,
                OutputFile = Path.GetTempFileName()
            };
            resultCode = assembleStep.Run(null);
            if (resultCode != 0)
            {
                Console.Error.WriteLine("Error occurred on Assemble step.  Exiting...");
                return resultCode;
            }

            Linker linkStep = new Linker
            {
                SourceFiles = new string[] { Pipeline.AssembledArtifact },
                OutputFile = Path.GetTempFileName(),
                LinkTarget = Target
            };
            resultCode = linkStep.Run(null);
            if (resultCode != 0)
            {
                Console.Error.WriteLine("Error occurred on Link step.  Exiting...");
                return resultCode;
            }

            Overlay overlayStep = new Overlay
            {
                InputFiles = new string[] { Pipeline.LinkedArtifact },
                OverlayPath = OutputFile,
                ShowInitAddress = true
            };
            resultCode = overlayStep.Run(null);
            if (resultCode != 0)
            {
                Console.Error.WriteLine("Error occurred on Overlay step.  Exiting...");
                return resultCode;
            }

            return 0;
        }
    }
}
