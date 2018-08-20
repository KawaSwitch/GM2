# about KawaGM (Kawa Geometric Modeling)
簡単な形状処理テストアプリケーションです  
スプライン曲線/曲面をGLUTを用いて描画&操作します  
  
# ブランチ
このリポジトリはWindows用とLinux用(GM3とGM2)をブランチを切って共存開発/保守しています  
それぞれの2ブランチが各OS上での開発ブランチで現在GM2は保守のみです  
 
- master(GM3)  
Windows用の開発ブランチ  
VisualStudioでの開発/実行を想定  
機能実装の主軸ブランチでOpenGLバージョンの高い環境にも対応  
- GM2  
Linux用の開発ブランチ  
cmake, g++での開発/実行を想定  
[About GM2](./READMEold.md)  
    
**以下masterブランチのみに関して**
  
# 開発環境
OS: Windows 10  
IDE: Visual Studio 2017  

# 動作確認済み/推奨環境
OpenGLバージョン 1.5以上  
これ未満だと技術的な問題で動作がかなり遅くなります    
  
Windows 10 OSでのみ動作確認済み  
Visual Studio 2017を推奨  
  
# ビルドに必要なパッケージ
どちらもNuGetパッケージマネージャで取得できます  
- nupengl  
- dirent  

# 実行方法
GM3.slnをVisual Studioで開いたらソリューションエクスプローラ内のソリューションを右クリックして  
"Nugetパッケージマネージャの復元(G)"をしてください  
上記のパッケージが導入できます  
  
あとは普通にビルドして実行  
実行が目的ならReleaseモードが速いです  
