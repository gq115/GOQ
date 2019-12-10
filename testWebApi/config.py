import yaml

LISTEN_IP = "0.0.0.0"
LISTEN_PORT = 8879
ALOWER_IP = ["127.0.0.1"]
DBINFO={
    "ip":"",
    "DB":"",
    "User":"",
    "Pass":""
}


with open('config.yaml', encoding='utf-8') as f:
    resObj = yaml.load(f, Loader=yaml.FullLoader)
    LISTEN_IP = resObj["listen_ip"]
    LISTEN_PORT = resObj["listen_port"]

    ALOWER_IP = resObj["ip_list"]

    DBINFO["ip"] = resObj["DBInfo"]["ip"]
    DBINFO["DB"] = resObj["DBInfo"]["DB"]
    DBINFO["User"] = resObj["DBInfo"]["User"]
    DBINFO["Pass"] = resObj["DBInfo"]["Pass"]



if __name__ == "__main__":
    print( DBINFO )
    print( LISTEN_IP )
    print( LISTEN_PORT )
    print( ALOWER_IP )
