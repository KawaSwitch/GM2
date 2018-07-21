# about Geometric Modeling 2
簡単な形状処理テストアプリケーションです  
スプライン曲線/曲面をGLUTを用いて描画&操作します  

__GMリポジトリの数字で何が違うか?__  
GM1はWindows/VisualStudioで開発  
GM2はLinux/g++コンパイラで開発(GM1からの派生)  

# 開発環境
Linux Mint Cinnamon 19  
g++ 7.3.0

# ビルドに必要なパッケージ
_Debian系OSの場合です_  
freeglut  
glew  
build-essential  
cmake  
とか

# 実行方法
cmakeの生成ファイルが邪魔なので以下のようにするのがおすすめです  
```
git clone <これのurl>
cd GM2/src
mkdir build
cd build
cmake ..
make
```
多分g++があれば動くはず
