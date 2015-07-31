Known Issue
-----------

Due to a difference in default keylengths between Windows Millennium/Windows XP and other
versions of Windows, the pvkimprt.exe utility may fail when used to import keys between
Millennium or XP and other Windows platforms.  The following is a workaround:

- Use pvkimprt.exe to export the .pvk and .spc files as type .pfx on a version of Windows
where the keys were generated (i.e. if the keypair was generated on Windows Millennium or XP, 
perform the .pfx export on a Millennium or XP machine).
- Once the keys have been exported as type .pfx, they can be imported on any Windows
platform using a certificate import tool (e.g. Certificate Import Wizard).

