

def write_spreadsheet():

    import psutil
    battery_SOC = psutil.sensors_battery().percent

    import requests
    url = "https://script.google.com/macros/s/AKfycbz_1fUKosiZFR1mf5dMq6L-DtUpQuCCHIvL_a3sIX953yWbBo1_Rvp4b5NISWmiCWEkGA/exec"

    payload = {"pParam1": battery_SOC}

    response = requests.get(url, params=payload)

    response.text
    response.status_code


def read_spreadsheet():

    # https://qiita.com/164kondo/items/eec4d1d8fd7648217935
    # https://tanuhack.com/library-gspread/#i-8

    import gspread
    import json
    from oauth2client.service_account import ServiceAccountCredentials

    # (1) Google Spread Sheetsにアクセス
    def connect_gspread(jsonf,key):
        scope = ['https://spreadsheets.google.com/feeds','https://www.googleapis.com/auth/drive']
        credentials = ServiceAccountCredentials.from_json_keyfile_name(jsonf, scope)
        gc = gspread.authorize(credentials)
        SPREADSHEET_KEY = key
        worksheet = gc.open_by_key(SPREADSHEET_KEY).sheet1
        return worksheet

    # ここでjsonfile名と2-2で用意したkeyを入力
    jsonf = "gspread-sheets-python-327914-63c440dad6f9.json"
    spread_sheet_key = "1D7iIKz-Dnt2JKzd20GJnGJkc1fnnvB8TRYG2QycdH94"
    ws = connect_gspread(jsonf,spread_sheet_key)

    cell_value = ws.acell('A1').value
    print(cell_value)


    cell_value = ws.acell('A2').value
    print(cell_value)

if __name__ == '__main__':
    write_spreadsheet()