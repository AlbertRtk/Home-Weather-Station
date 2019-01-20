""" Albert Ratajczak, 2019
"""
import pdb

from google.oauth2.service_account import Credentials
from googleapiclient.discovery import build
from pprint import pprint

import kivy
kivy.require('1.10.1')

from kivy.uix.floatlayout import FloatLayout
from kivy.app import App
from kivy.properties import ObjectProperty

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


class Weather(FloatLayout):
    temperature = ObjectProperty()
    humidity = ObjectProperty()
    pressure = ObjectProperty()
    last_update = ObjectProperty()

    def update(self):
        weather = recent_weather()
        self.temperature.text = weather['Temperature'] + ' °C'
        self.humidity.text = weather['Humidity'] + '%'
        self.pressure.text = weather['Pressure'] + ' hPa'
        self.last_update.text = 'Updated: ' + weather['Date']


class WeatherApp(App):
    def build(self):
        return Weather()


if __name__ == '__main__':
    WeatherApp().run()
