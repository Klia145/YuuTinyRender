Write-Host "Cleaning build files..." -ForegroundColor Yellow
Remove-Item -Recurse -Force build_cmake -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
Write-Host "Done!" -ForegroundColor Green