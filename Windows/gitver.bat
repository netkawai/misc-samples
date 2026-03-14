@echo on

echo #ifndef GITVER_H > gitver.h
echo #define GITVER_H >> gitver.h
:: Output "abbv-month date" to follow __DATE__ of Month Day
for /f "delims=" %%i in ('git  log -1 --format"="%%cd --date"="format:"%%b %%d"') do set GIT_COMMIT_DATE=%%i
echo #define GIT_COMMIT_DATE "%GIT_COMMIT_DATE%" >> gitver.h
echo #endif >> gitver.h
