@echo off

if "%1"=="pull" (
    git pull origin master
    cd submodules/gd.h
    git pull origin main
    cd ../../submodules/cocos2d
    git pull origin master

    goto done
)

echo ==== BetterGD ====
echo.

git add --all
git commit -a
git push origin master

rem one of these will fail and the other'll work for me lol

cd submodules

echo.
echo ==== gd.h ====
echo.

cd gd.h

git add --all
git commit -a
git push origin main

echo.
echo ==== Cocos2d ====
echo.

cd ..\cocos2d

git add --all
git commit -a
git push origin master

:done
