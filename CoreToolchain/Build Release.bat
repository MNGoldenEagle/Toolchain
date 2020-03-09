@ECHO OFF
dotnet build -f net462 -c Release -r win10-x64
dotnet build -f netcoreapp2.1 -c Release -r osx-x64
dotnet build -f netcoreapp2.1 -c Release -r linux-x64
dotnet publish -f net462 -c Release -r win10-x64 /p:TrimUnusedDependencies=true
dotnet publish -f netcoreapp2.1 -c Release -r osx-x64 /p:TrimUnusedDependencies=true
dotnet publish -f netcoreapp2.1 -c Release -r linux-x64 /p:TrimUnusedDependencies=true
robocopy ".\Binaries-Win10" "bin\Release\net462\win10-x64\publish\Binaries" *.* /NP /NJH
robocopy ".\Binaries-OSX" "bin\Release\netcoreapp2.1\osx-x64\publish\Binaries" *.* /NP /NJH
robocopy ".\Binaries-Linux" "bin\Release\netcoreapp2.1\linux-x64\publish\Binaries" *.* /NP /NJH
robocopy "..\Z64 Headers\Templates" "bin\Release\net462\win10-x64\publish\Templates" *.csv /NP /NJH
robocopy "..\Z64 Headers\Templates" "bin\Release\netcoreapp2.1\osx-x64\publish\Templates" *.csv /NP /NJH
robocopy "..\Z64 Headers\Templates" "bin\Release\netcoreapp2.1\linux-x64\publish\Templates" *.csv /NP /NJH
robocopy "..\Z64 Headers" "bin\Release\net462\win10-x64\publish\Headers" *.h /NP /NJH
robocopy "..\Z64 Headers" "bin\Release\netcoreapp2.1\osx-x64\publish\Headers" *.h /NP /NJH
robocopy "..\Z64 Headers" "bin\Release\netcoreapp2.1\linux-x64\publish\Headers" *.h /NP /NJH
robocopy "..\Z64 Examples\src" "bin\Release\net462\win10-x64\publish\Examples" *.* /E /NP /NJH
robocopy "..\Z64 Examples\src" "bin\Release\netcoreapp2.1\osx-x64\publish\Examples" *.* /E /NP /NJH
robocopy "..\Z64 Examples\src" "bin\Release\netcoreapp2.1\linux-x64\publish\Examples" *.* /E /NP /NJH
