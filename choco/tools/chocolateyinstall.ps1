$ErrorActionPreference = 'Stop'

$packageName = 'colorcop'
$toolsDir    = Split-Path -Parent $MyInvocation.MyCommand.Definition
$installer   = Join-Path $toolsDir 'colorcop-setup.exe'

$packageArgs = @{
    packageName    = $packageName
    fileType       = 'exe'
    file           = $installer
    silentArgs     = '/S'
    validExitCodes = @(0)
}

Install-ChocolateyInstallPackage @packageArgs

