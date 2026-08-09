# Smart Energy Monitoring & ML Anomaly Detection ⚡🤖

An engineering project combining **IoT-based electrical energy monitoring** with **machine learning for anomaly detection**.

## 🎯 Goal

Monitor electrical energy consumption from a single measurement point and use machine-learning techniques to identify abnormal consumption patterns.

## 🏗️ System Architecture

```text
Electrical Load
      │
      ▼
Current Sensing (SCT-013)
      │
      ▼
Signal Acquisition
      │
      ▼
ESP32
      │
      ▼
Energy Data
      │
      ▼
Preprocessing
      │
      ▼
Feature Engineering
      │
      ▼
Machine Learning
      │
      ▼
Anomaly Detection
      │
      ▼
Analysis / Visualization
```

## 🔧 Hardware

- ESP32 / ESP32-S3
- SCT-013 current transformer
- ADC / signal-conditioning components where required
- Electrical load under test

> **Safety:** mains electrical measurements require appropriate isolation, protection, supervision, and safe laboratory procedures.

## 🧠 ML Workflow

1. Collect electrical measurements
2. Clean and validate data
3. Explore consumption patterns
4. Engineer features
5. Prepare training/evaluation data
6. Train an anomaly-detection or classification model
7. Evaluate performance
8. Visualize abnormal patterns
9. Document findings and limitations

## 📁 Structure

```text
smart-energy-anomaly-detection/
├── data/
│   ├── raw/
│   └── processed/
├── hardware/
├── models/
├── notebooks/
├── results/figures/
├── src/
│   ├── preprocessing.py
│   ├── features.py
│   └── anomaly_detection.py
├── .gitignore
├── requirements.txt
└── README.md
```

## 🛠️ Technologies

**IoT:** ESP32, SCT-013

**Data/ML:** Python, NumPy, Pandas, Scikit-learn, Matplotlib, Jupyter

**Development:** Git, GitHub, VS Code

## 📊 Results

Model metrics and experiment results will be added after they are measured from the project's actual experiments. No unverified performance numbers are claimed here.

Potential evaluation metrics include precision, recall, F1-score, confusion matrix, ROC-AUC where appropriate, and false-positive/false-negative rates.

## 🚀 Future Improvements

- Real-time monitoring dashboard
- More robust anomaly detection
- Appliance/load signature analysis
- Automated alerts
- Cloud storage
- Model/API deployment
- Edge/cloud hybrid architecture
- Reproducible experiment tracking

## 👨🏽‍💻 Author

**Anuoluwapo Akinshe**

AI/ML Engineer | Python Developer | Electrical & Electronics Engineer

- GitHub: https://github.com/anuoluwaMLops
- LinkedIn: https://linkedin.com/in/anuoluwapo-akinshe
- Email: boluwatifeakinshe@gmail.com

**Project status:** Active development
