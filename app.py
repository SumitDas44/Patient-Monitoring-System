from flask import Flask, request, jsonify, render_template
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.preprocessing import LabelEncoder
import os

app = Flask(__name__)

# Global variables to hold the trained model and encoders
model = None
label_encoders = {}

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/upload', methods=['POST'])
def upload_csv():
    global model, label_encoders

    if 'file' not in request.files:
        return jsonify({"error": "No file part"}), 400

    file = request.files['file']
    if file.filename == '':
        return jsonify({"error": "No selected file"}), 400

    # Read CSV file
    try:
        df = pd.read_csv(file)
        
        # Basic preprocessing
        if any(col not in df.columns for col in ["Body Temperature (\u00b0C)", "Heart Rate (BPM)", "Systolic Pressure (mmHg)", "Health Status"]):
            return jsonify({"error": "CSV file must contain required columns."}), 400

        X = df[["Body Temperature (°C)", "Heart Rate (BPM)", "Systolic Pressure (mmHg)"]]
        y = df["Health Status"]

        # Encode labels
        label_encoders['Health Status'] = LabelEncoder()
        y_encoded = label_encoders['Health Status'].fit_transform(y)

        # Train/test split
        X_train, X_test, y_train, y_test = train_test_split(X, y_encoded, test_size=0.2, random_state=42)

        # Train the model
        model = DecisionTreeClassifier()
        model.fit(X_train, y_train)

        accuracy = model.score(X_test, y_test)

        return jsonify({"message": "Model trained successfully", "accuracy": accuracy}), 200

    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/predict', methods=['POST'])
def predict():
    global model, label_encoders

    if model is None:
        return jsonify({"error": "Model is not trained yet."}), 400

    try:
        # Parse input data
        data = request.json
        temp = data.get("body_temperature")
        bpm = data.get("heart_rate")
        systolic = data.get("systolic_pressure")

        if temp is None or bpm is None or systolic is None:
            return jsonify({"error": "Missing input values."}), 400

        # Predict
        input_features = [[temp, bpm, systolic]]
        prediction = model.predict(input_features)
        health_status = label_encoders['Health Status'].inverse_transform(prediction)[0]

        return jsonify({"health_status": health_status}), 200

    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    # Ensure templates folder exists
    if not os.path.exists('templates'):
        os.makedirs('templates')

    # Save a simple HTML template
    with open('templates/index.html', 'w') as f:
        f.write("""<!DOCTYPE html>
<html>
<head>
    <title>Health Monitoring App</title>
</head>
<body>
    <h1>Upload Health Dataset</h1>
    <form action="/upload" method="post" enctype="multipart/form-data">
        <input type="file" name="file">
        <button type="submit">Upload</button>
    </form>
</body>
</html>
""")

    app.run(debug=True)