import requests,time
from bs4 import BeautifulSoup

id_variable = "Your id"
password_variable = "Your password"
id_host = "https://fw-cgcp.emse.fr" #do not modify for mines de St

url = id_host + "auth/plain.html"
headers = {
    "Host": id_host,
    "Content-Length": "72",
    "Cache-Control": "max-age=0",
    "Sec-Ch-Ua": "",
    "Sec-Ch-Ua-Mobile": "?0",
    "Sec-Ch-Ua-Platform": "",
    "Upgrade-Insecure-Requests": "1",
    "Origin": id_host,
    "Content-Type": "application/x-www-form-urlencoded",
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.5790.110 Safari/537.36",
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
    "Sec-Fetch-Site": "same-origin",
    "Sec-Fetch-Mode": "navigate",
    "Sec-Fetch-User": "?1",
    "Sec-Fetch-Dest": "document",
    "Referer": id_host+ "/auth/auth.html?authnum=1&uid=balek&time=480&pswd=&logout=&url=",
    "Accept-Encoding": "gzip, deflate",
    "Accept-Language": "en-GB,en-US;q=0.9,en;q=0.8",
    "Connection": "close"
}

data = {
    "url": "",
    "logout": "",
    "time": "480",
    "uid": id_variable,
    "authnum": "1",
    "pswd": password_variable
}



def get_id(html_content):
    try:
        soup = BeautifulSoup(html_content, 'html.parser')
        div_tag = soup.find('div', {'class': 'form-group', 'hidden': ''})
        input_tag = div_tag.find('input', {'name': 'session'})
        value = input_tag.get('value')
        # Séparation de la chaîne de caractères pour obtenir l'ID
        id_str = value.split('=')[1]
        return value
    except Exception as e:
        print(f"Failes getting ID : {e}")
        return None


def accept(id):
    url = id_host+ "auth/disclaimer.html"
    params = {
        "form1": "",
        "form2": "",
        "form3": "",
        "session": id,
        "url": "",
        "time": "",
        "authnum": "",
        "read": "accepted",
        "action": "I accept"
    }

    headers = {
        "Host": id_host,
        "Sec-Ch-Ua": "",
        "Sec-Ch-Ua-Mobile": "?0",
        "Sec-Ch-Ua-Platform": "",
        "Upgrade-Insecure-Requests": "1",
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.5790.110 Safari/537.36",
        "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
        "Sec-Fetch-Site": "same-origin",
        "Sec-Fetch-Mode": "navigate",
        "Sec-Fetch-User": "?1",
        "Sec-Fetch-Dest": "document",
        "Referer": id_host+ "/auth/plain.html",
        "Accept-Encoding": "gzip, deflate",
        "Accept-Language": "en-GB,en-US;q=0.9,en;q=0.8",
    }

    try:
        response = requests.get(url, params=params, headers=headers)
        if response.status_code == 200:
            soup = BeautifulSoup(response.text, 'html.parser')
            # Now you can work with the parsed HTML (soup) here
            return soup
        else:
            print(f"Failed to send GET request. Status code: {response.status_code}")
    except Exception as e:
        print(f"An error occurred: {str(e)}")



def login():
    response = requests.post(url, headers=headers, data=data)
    if response.status_code == 200:
        soup = BeautifulSoup(response.text, 'html.parser')
        s = str(soup)
        if "Welcome" in s:
            return 0
        elif "Internet" in s:
            long_id_input = soup.find('input', {'name': 'session'})
            long_id = long_id_input.get('value')
            accept(long_id)
            return 0
        else:
            return -1



def logged_in():
    url = id_host+"auth/"
    try:
        response = requests.get(url)
        if response.status_code == 200:
            page_content = response.text
            if id_variable in page_content:
                return True
            else:
                return False
        else:
            print(f"Failed to fetch the webpage. Status code: {response.status_code}")
            return False
    except Exception as e:
        print(f"An error occurred: {str(e)}")
        return False

while True:
    if not logged_in():
        login()
    time.sleep(5)

