import os
import json
from datetime import datetime, timedelta, timezone

from flask import Flask, render_template, jsonify
import firebase_admin
from firebase_admin import credentials, firestore
from dotenv import load_dotenv


# Load environment variables
load_dotenv()

app = Flask(__name__)

# Firebase configuration
SERVICE_ACCOUNT_PATH = os.getenv(
    "FIREBASE_SERVICE_ACCOUNT",
    "serviceAccountKey.json"
)

if not firebase_admin._apps:
    cred = credentials.Certificate(SERVICE_ACCOUNT_PATH)
    firebase_admin.initialize_app(cred)

db = firestore.client()


@app.route("/")
def dashboard():
    """Render the energy monitoring dashboard."""

    cutoff_time = datetime.now(timezone.utc) - timedelta(hours=24)

    docs = (
        db.collection("measurements")
        .where("timestamp", ">=", cutoff_time)
        .order_by("timestamp")
        .stream()
    )

    measurements = [doc.to_dict() for doc in docs]

    alert_docs = (
        db.collection("alerts")
        .order_by(
            "timestamp",
            direction=firestore.Query.DESCENDING
        )
        .limit(10)
        .stream()
    )

    alerts = [doc.to_dict() for doc in alert_docs]

    if measurements:
        powers = [m.get("power", 0) for m in measurements]
        voltages = [m.get("voltage", 0) for m in measurements]

        stats = {
            "avg_power": sum(powers) / len(powers),
            "max_power": max(powers),
            "min_power": min(powers),
            "avg_voltage": sum(voltages) / len(voltages),
            "total_measurements": len(measurements),
            "total_alerts": len(alerts),
        }
    else:
        stats = {
            "avg_power": 0,
            "max_power": 0,
            "min_power": 0,
            "avg_voltage": 0,
            "total_measurements": 0,
            "total_alerts": len(alerts),
        }

    return render_template(
        "dashboard.html",
        measurements=measurements,
        alerts=alerts,
        stats=stats,
    )


@app.route("/api/data")
def api_data():
    """Return the latest 50 energy measurements as JSON."""

    docs = (
        db.collection("measurements")
        .order_by(
            "timestamp",
            direction=firestore.Query.DESCENDING
        )
        .limit(50)
        .stream()
    )

    data = [doc.to_dict() for doc in docs]

    return jsonify(data)


if __name__ == "__main__":
    app.run(
        host="0.0.0.0",
        port=5000,
        debug=True
    )
