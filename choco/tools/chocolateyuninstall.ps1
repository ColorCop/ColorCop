$ErrorActionPreference = 'Stop'

$packageName = 'colorcop'

$packageArgs = @{
    packageName    = $packageName
    fileType       = 'exe'
    silentArgs     = '/VERYSILENT /SUPPRESSMSGBOXES /NORESTART /SP-'
    validExitCodes = @(0)
}

Uninstall-ChocolateyPackage @packageArgs
