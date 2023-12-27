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

## プロジェクト名

VG-Hub

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
├── .DS_Store  
├── .gitignore  
├── .vscode  
│   └── settings.json  
├── README.md  
├── VirtualGridHub  
│   ├── .DS_Store  
│   ├── data_collection  
│   ├── devices_classify  
│   └── main  
├── maxflow  
│   ├── addbottom_vghub4_3.csv  
│   ├── addright_vghub4_3.csv  
│   ├── maxflowderive_proposition.py  
│   ├── maxflowderive_proposition_old.py  
│   ├── maxflowderive_sequentially.py  
│   ├── sink_vghub4_3.csv  
│   ├── source_vghub4_3.csv  
│   └── vertexes4.csv  
└── visualization  
    ├── csv  
    ├── googlepixel3a#1.ipynb  
    └── ipadair4th#1.ipynb  
```
<p align="right">(<a href="#top">トップへ</a>)</p>

## 開発環境構築

<p align="right">(<a href="#top">トップへ</a>)</p>

