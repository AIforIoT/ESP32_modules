import http.client
import sys

def main():
    url = "172.20.10.10"
    conn = http.client.HTTPConnection(url, 80, timeout=10)

    try:
        if(sys.argv[1] == "H"):
            conn.request("GET", "/H")
            responseH = conn.getresponse()
            print(responseH.status, responseH.reason)

        else if(sys.argv[1] == "H"):
            statusL = conn.request("GET", "/L")
            responseL = conn.getresponse()
            print(responseL.status, responseL.reason)

        else if(sys.argv[1] == "data_on"):
            dataOn = conn.request("GET", "/data/on")
            responseOn = conn.getresponse()
            print(responseOn.status, responseOn.reason)
        conn.close()
    except:"Connection not Established"

    return 0

if __name__ == '__main__':
    main()
