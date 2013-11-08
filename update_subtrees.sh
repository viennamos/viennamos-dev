git fetch viennamath master
git subtree pull --prefix external/ViennaMath viennamath master --squash

git fetch viennamini master
git subtree pull --prefix external/ViennaMini viennamini master --squash

git fetch viennafvm master
git subtree pull --prefix external/ViennaFVM viennafvm master --squash

git fetch viennadata master
git subtree pull --prefix external/ViennaData viennadata master --squash

git fetch viennagrid master
git subtree pull --prefix external/ViennaGrid viennagrid master --squash
