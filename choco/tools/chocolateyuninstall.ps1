$ErrorActionPreference = 'Stop'

$packageName = 'colorcop'

$packageArgs = @{
    packageName    = $packageName
    fileType       = 'exe'
    silentArgs     = '/S'
    validExitCodes = @(0)
}

Uninstall-ChocolateyPackage @packageArgs

