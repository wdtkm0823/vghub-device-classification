# デバイス識別
## Pythonの仮想環境
### 仮想環境作成
プロジェクト直下に移動後、以下のコマンドを実行するとプロジェクト内に指定した仮想環境名のフォルダが作成される。  
`python -m venv venv`
### 仮想環境のアクティベート
次に、以下のコマンドを使って仮想環境をアクティベートする。
#### Linux, Macの場合
`. venv/bin/activate.fish`
#### Windowsの場合
`. venv\Scripts\activate`

### パッケージのインストール  
`pip install -r requirements.txt`

#### tensorflowをインストールできない場合
tensorflowをURLからインストールする。
- python 3.11
pip install --upgrade https://storage.googleapis.com/tensorflow/windows/cpu/tensorflow_cpu-2.15.0-cp311-cp311-win_amd64.whl
