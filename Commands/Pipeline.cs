using ManyConsole;
using System.Collections.Generic;
using System;

namespace Toolchain
{
    internal static class Pipeline
    {
        internal static string[] CompiledArtifacts = { };

        internal static string AssembledArtifact { get; set; }

        internal static string LinkedArtifact { get; set; }
    }
}
