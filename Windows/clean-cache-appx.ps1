
# Get-AppxPackage | Where -Property Name -like "Local" | Remove-AppxPackage

Get-AppxPackage | Where -Property InstallLocation -like "*Cache*" | Remove-AppxPackage