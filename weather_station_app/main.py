""" Albert Ratajczak, 2019
"""

import kivy
kivy.require('1.10.1')

from kivy.uix.floatlayout import FloatLayout
from kivy.app import App
from kivy.properties import ObjectProperty
from kivy.clock import Clock

from google.oauth2.service_account import Credentials
from googleapiclient.discovery import build


# Global variables for connection with Google Sheets
SERVICE_ACCOUNT = 'service.json'
SCOPES = ['https://www.googleapis.com/auth/spreadsheets.readonly']
SPREADSHEET_ID = '1XR3SdMlfMPe3wa8Ch3fqPpbLMGnmUfp5aqusvrv2YeI'
RANGE_NAME = 'Data!A2:E'

# Getting credential for service account from JSON file
CREDS = Credentials.from_service_account_file(SERVICE_ACCOUNT, scopes=SCOPES)
SERVICE = None  # will be build after pressing Update button
                # > recent_weather > get_weather_data > build_global_service


def build_global_service():
    """
    Builds connection to Google Sheets
    """
    global SERVICE
    SERVICE = build('sheets', 'v4', credentials=CREDS)


def get_weather_data():
    """
    Downloads values from Google Sheet defined by SPREADSHEET_ID
    Values range: RANGE_NAME

    :return: table with values
    """
    if not SERVICE: build_global_service()
    return SERVICE.spreadsheets().values() \
           .get(spreadsheetId=SPREADSHEET_ID, range=RANGE_NAME) \
           .execute()['values']


def recent_weather():
    """
    Selects last raw from table with values returned by function
    get_weather_data() and creates dictionary with last recorded weather info

    :return: dictionary with weather info
    """
    weather_data = get_weather_data()
    temperature = (float(weather_data[-1][1])+float(weather_data[-1][2])) / 2
    return {'Date': weather_data[-1][0],
            'Temperature': str(temperature),
            'Pressure': weather_data[-1][3],
            'Humidity': weather_data[-1][4]}


class Weather(FloatLayout):
    """
    Layout class of the app
    """
    temperature = ObjectProperty()
    humidity = ObjectProperty()
    pressure = ObjectProperty()
    last_update = ObjectProperty()

    def update(self):
        """
        Updates weather info and labels. Triggered by Update button
        """
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
