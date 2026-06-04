$ErrorActionPreference = 'Stop'

$packageName = 'colorcop'
$toolsDir    = Split-Path -Parent $MyInvocation.MyCommand.Definition
$installer   = Join-Path $toolsDir 'colorcop-setup.exe'

$packageArgs = @{
    packageName    = $packageName
    fileType       = 'exe'
    file           = $installer
    silentArgs     = '/VERYSILENT /SUPPRESSMSGBOXES /NORESTART /SP-'
    validExitCodes = @(0)
}

Uninstall-ChocolateyPackage @packageArgs
