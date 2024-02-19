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

## Virtual Grid

小規模創電・蓄電の統合運用を可能にするインターネット型電力プラットフォーム  
本研究はJSPS科研費23H01636の助成を受けたものです。

### 参考URL 
インターネット型電力プラットフォームについて：　http://www.iperc.uec.ac.jp/virtualgrid.html

<!-- プロジェクトについて -->

## プロジェクトについて

USB-PDに準拠したUSB Type-Cに接続されたデバイス情報の取得

## 環境

| 言語・フレームワーク  | バージョン |
| --------------------- | ---------- |
| Python                | 3.11.3     |

<p align="right">(<a href="#top">トップへ</a>)</p>

## ディレクトリ構成

<!-- Treeコマンドを使ってディレクトリ構成を記載 -->

❯ tree -a -I "node_modules|.next|.git|.pytest_cache|static" -L 2
```
.  
.
├── .DS_Store
├── .gitignore
├── .venv
│   ├── .gitignore
│   ├── bin
│   ├── include
│   ├── lib
│   ├── pyvenv.cfg
│   └── share
├── MaxFlowGraph
│   ├── addbottom_vghub4_3.csv
│   ├── addright_vghub4_3.csv
│   ├── maxflowderive_proposition.py
│   ├── maxflowderive_proposition_old.py
│   ├── maxflowderive_sequentially.py
│   ├── sink_vghub4_3.csv
│   ├── source_vghub4_3.csv
│   └── vertexes4.csv
├── README.md
├── SECURITY.md
├── VirtualGridHub
│   ├── .DS_Store
│   ├── DataCollection
│   ├── DevicesClassify
│   └── main
├── VisualizationWave
│   ├── .DS_Store
│   ├── Images
│   ├── README.md
│   ├── ankerpowercore3#1.ipynb
│   ├── cheeropowermountain#1.ipynb
│   ├── cheeropowerplus5#1.ipynb
│   ├── googlepixel3a#1.ipynb
│   ├── ipadair4th#1.ipynb
│   ├── omnichargeomni20+#1.ipynb
│   ├── requirements.txt
│   └── xperiaxz2compact#1.ipynb
└── dependabot.yml 
```
<p align="right">(<a href="#top">トップへ</a>)</p>

## 開発環境構築

<p align="right">(<a href="#top">トップへ</a>)</p>

