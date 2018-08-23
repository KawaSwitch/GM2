# about Geometric Modeling 2
簡単な形状処理テストアプリケーションです  
スプライン曲線/曲面をGLUTを用いて描画&操作します  
  
# 開発環境
Linux Mint 19 Cinnamon  
g++ 7.3.0

# 動作確認済み/推奨環境
OpenGLバージョン 1.5以上  
これ未満だと技術的な問題で動作が遅くなります  

g++ 5.1以上  
C++14を使うため  

確認済OS  
- Linux  
  * Linux Mint 19 Cinnamon  
  * Linux Mint 19 MATE  
  * Ubuntu 16.04 LTS  
  * Ubuntu 18.04 LTS  
  * CentOS 7.5.1804
- Windows  
  * VirtualBox, VMwareで上記のディストリビューション  
  * WSL(Ubuntu 18.04 LTS)  
  
Linux以外からは仮想環境のみ  
Windowsの場合WSLが楽  
  
# ビルドに必要なパッケージ
(Debian系OSの場合)  
freeglut  
glew  
build-essential  
cmake  
とか  

# 実行方法
cmakeの生成ファイルが邪魔なので以下のようにするのがおすすめです  
```  
git clone <これのurl>  
cd GM2  
mkdir build  
cp -r ./KJS_FILE ./build  
cd build  
cmake ..  
make  
./geoModeling  
```  
