import paho.mqtt.client as mqtt
import pymysql
import time

# Configuration de la connexion MariaDB
try:
    db = pymysql.connect(
        host="localhost",
        user="XXXXX",
        password="XXXXX",
        database="portail_logs"
    )
    cursor = db.cursor()
    print("Connecté à MariaDB avec succès")
except Exception as e:
    print(f"Erreur de connexion DB : {e}")
    exit()

def on_message(client, userdata, message):
    event = str(message.payload.decode("utf-8"))
    # On définit un nom de photo avec le temps pour l'unicité
    img_name = f"capture_{int(time.time())}.jpg"
    
    try:
        # Correspondance avec ta table 'historique'
        sql = "INSERT INTO historique (evenement, statut, photo_nom) VALUES (%s, %s, %s)"
        val = (event, "SUCCES", img_name)
        cursor.execute(sql, val)
        db.commit()
        print(f"Log inséré : {event} | Image : {img_name}")
    except Exception as e:
        print(f"Erreur lors de l'insertion : {e}")

# Configuration MQTT
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION1, "RPI_Bridge_Logger")
client.on_message = on_message

print("En attente de messages MQTT...")
client.connect("localhost", 1883)
client.subscribe("portail/logs")

client.loop_forever()