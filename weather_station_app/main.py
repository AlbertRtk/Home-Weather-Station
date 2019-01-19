""" Albert Ratajczak, 2019
"""
import pdb

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


def get_weather_data():
    return SERVICE.spreadsheets().values() \
           .get(spreadsheetId=SPREADSHEET_ID, range=RANGE_NAME) \
           .execute()['values']


def recent_weather():
    weather_data = get_weather_data()
    temperature = (float(weather_data[-1][1])+float(weather_data[-1][2])) / 2
    return {'Date': weather_data[-1][0],
            'Temperature': str(temperature),
            'Pressure': weather_data[-1][3],
            'Humidity': weather_data[-1][4]}


if __name__ == '__main__':
    # pdb.set_trace()
    pprint(recent_weather())
