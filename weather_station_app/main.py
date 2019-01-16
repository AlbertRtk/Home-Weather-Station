from google.oauth2.service_account import Credentials
from googleapiclient.discovery import build
from pprint import pprint

print('Weather Station App')
SERVICE_ACCOUNT = 'service.json'
SCOPES = ['https://www.googleapis.com/auth/spreadsheets.readonly']
SPREADSHEET_ID = '1XR3SdMlfMPe3wa8Ch3fqPpbLMGnmUfp5aqusvrv2YeI'
RANGE_NAME = 'Data!A2:E'

print('building service')
CREDS = Credentials.from_service_account_file(SERVICE_ACCOUNT, scopes=SCOPES)
SERVICE = build('sheets', 'v4', credentials=CREDS)

WEATHER_DATA = []

def update_weather_data():
    global WEATHER_DATA
    sheet = SERVICE.spreadsheets()
    request = sheet.values().get(spreadsheetId=SPREADSHEET_ID, range=RANGE_NAME)
    WEATHER_DATA = request.execute()['values']


def recent_weather():
    temperature = (float(WEATHER_DATA[-1][1])+float(WEATHER_DATA[-1][2])) / 2
    return {'Date': WEATHER_DATA[-1][0], 'Temperature': str(temperature),
            'Pressure': WEATHER_DATA[-1][3], 'Humidity': WEATHER_DATA[-1][4]}


if __name__ == '__main__':
    update_weather_data()
    pprint(recent_weather())
