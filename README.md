<div id="top"></div>

## 使用技術一覧

<p style="display: inline">
  <img src="https://img.shields.io/badge/-Python-F9DC3E.svg?logo=python&style=flat">
  <img src="https://img.shields.io/badge/-C-555.svg?logo=c&style=flat">
</p>

## 目次

1. [プロジェクトについて](#プロジェクトについて)
2. [環境](#環境)
3. [ディレクトリ構成](#ディレクトリ構成)
4. [開発環境構築](#開発環境構築)

## プロジェクトについて

### Virtual Grid

小規模創電・蓄電の統合運用を可能にするインターネット型電力プラットフォーム  
本研究はJSPS科研費23H01636の助成を受けたものです。

#### 参考URL 
インターネット型電力プラットフォームについて：　http://www.iperc.uec.ac.jp/virtualgrid.html

### 目的
USB-PDに準拠したUSB Type-Cに接続されたデバイス識別

## 環境

| 言語・フレームワーク  | バージョン |
| --------------------- | ---------- |
| Python                | 3.11.3     |
| C                     |            |

## ディレクトリ構成

<!-- Treeコマンドを使ってディレクトリ構成を記載  
`
❯ tree -a -I "node_modules|.next|.git|.pytest_cache|static" -L 2
` -->
```
.
├── .DS_Store
├── .gitignore
├── .venv
│   ├── .gitignore
│   ├── bin
│   ├── include
│   ├── lib
│   ├── pyvenv.cfg
│   └── share
├── README.md
├── SECURITY.md
├── VirtualGridHub
│   ├── .DS_Store
│   ├── DataCollection
│   ├── deviceclassify
│   └── main
├── dependabot.yml
├── requirements.txt
└── visualization_wave
    ├── .DS_Store
    ├── ankerpowercore3#1.ipynb
    ├── cheeropowermountain#1.ipynb
    ├── cheeropowerplus5#1.ipynb
    ├── googlepixel3a#1.ipynb
    ├── images
    ├── ipadair4th#1.ipynb
    └── xperiaxz2compact#1.ipynb
```
<p align="right">(<a href="#top">トップへ</a>)</p>

## 開発環境構築
### Pythonの仮想環境
#### 仮想環境作成
プロジェクト直下に移動後、以下のコマンドを実行するとプロジェクト内に指定した仮想環境名のフォルダが作成される。  
`python -m venv .venv`
#### 仮想環境のアクティベート
次に、以下のコマンドを使って仮想環境をアクティベートする。
##### Linux, Macの場合
`. .venv/bin/activate.fish`
##### Windowsの場合
`. .venv\Scripts\activate`

#### パッケージのインストール  
`pip install -r requirements.txt`

##### tensorflowをインストールできない場合
tensorflowをURLからインストールする。
- python 3.11  
`pip install --upgrade https://storage.googleapis.com/tensorflow/windows/cpu/tensorflow_cpu-2.15.0-cp311-cp311-win_amd64.whl`
<p align="right">(<a href="#top">トップへ</a>)</p>

