
import joblib

# Load trained model
model = joblib.load("battery_ai_model.pkl")

# User input
temp = float(input("Enter temperature (°C): "))
volt = float(input("Enter voltage (V): "))

# Make prediction
prediction = model.predict([[temp, volt]])

if prediction[0] == 1:
    print("⚠️ AI Alert: Battery anomaly detected (risk of failure)!")
else:
    print("✅ AI: Battery condition appears normal.")
