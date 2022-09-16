# only Remove Appruntime (not dynamic dependency manager nor singleton)
Get-AppxPackage | Where -Property Name -like "Microsoft.WindowsAppRuntime.*" | Remove-AppxPackage)