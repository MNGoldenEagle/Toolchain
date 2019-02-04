using ManyConsole;
using System.Collections.Generic;
using System;

namespace Toolchain
{
    class Program
    {
        private static readonly IEnumerable<ConsoleCommand> Commands = GetCommands();

        private static IEnumerable<ConsoleCommand> GetCommands()
        {
            return ConsoleCommandDispatcher.FindCommandsInAllLoadedAssemblies();
        }

        static int Main(string[] args)
        {
            return ConsoleCommandDispatcher.DispatchCommand(Commands, args, Console.Out);
        }
    }
}
