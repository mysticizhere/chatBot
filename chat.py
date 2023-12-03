import json
import numpy as np
import warnings
warnings.filterwarnings("ignore")
from tensorflow import keras
from tensorflow.keras.preprocessing.sequence import pad_sequences
from sklearn.preprocessing import LabelEncoder
import colorama
colorama.init()
from colorama import Fore, Style
import pickle

# Load intents.json and the trained model
with open('intents.json') as file:
    data = json.load(file)

model = keras.models.load_model('chat-model')

with open('tokenizer.pickle', 'rb') as handle:
    tokenizer = pickle.load(handle)

with open('label_encoder.pickle', 'rb') as enc:
    lbl_encoder = pickle.load(enc)

# Chat function
def chat():
    while True:
        print('User: ', end="")
        user_input = input()

        if user_input.lower() == 'quit':
            print('CareBot: Take care. See you soon.')
            break

        # Predict using the model
        sequences = tokenizer.texts_to_sequences([user_input])
        padded_sequence = pad_sequences(sequences, truncating='post', maxlen=128)
        result = model.predict(padded_sequence)
        predicted_tag = lbl_encoder.inverse_transform([np.argmax(result)])[0]

        # Retrieve responses based on the predicted tag
        for intent in data['intents']:
            if intent['tag'] == predicted_tag:
                responses = intent['responses']
                print('CareBot:', np.random.choice(responses))
        #     else:
        #         print('CareBot:', 'I am sorry I did not understand you, try to rephrase your statement')
        # for intent in data['intents']:
        #     if intent['tag'] == predicted_tag:
        #         responses = intent['responses']
        #         print('CareBot:', np.random.choice(responses))
        #         break
        # else:
        #     print('CareBot:', 'I am sorry I did not understand you, try to rephrase your statement')


print('Start talking with CareBot. (Type quit to stop talking)')
chat()
