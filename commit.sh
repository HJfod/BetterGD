
if [ $1 == "pull" ]; then
    git pull origin master
    cd submodules/gd.h
    git pull origin main
    cd ../../submodules/cocos2d
    git pull origin master
else

    echo "==== BetterGD ===="
    echo ""

    git add --all
    git commit -a
    git push origin master

    cd submodules

    echo ""
    echo "==== GD.H ===="
    echo ""

    cd gd.h

    git add --all
    git commit -a
    git push origin main

    echo ""
    echo "==== Cocos2d ===="
    echo ""

    cd ../cocos2d

    git add --all
    git commit -a
    git push origin master

fi
