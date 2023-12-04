# run using " streamlit run app_streamlit.py"

# app.py
import streamlit as st
from chat import chat_function
import time

def main():
    # Streamlit setup
    st.set_page_config(page_title="Streamlit Python App")

    # Input section
    st.header("Drug Relief Chatbot")

    # Initialize or load conversation history
    conversation = st.session_state.get("conversation", [])

    user_input = st.text_input("You:", key="user_input")

    if st.button("ASK ME"):
        if user_input.strip() == "":
            st.warning("Please enter a message.")
        # Display user message
        conversation.append(f"You: {user_input}")

        # Simulate processing time
        with st.spinner("Don't Worry !! I am searching for help..."):
            time.sleep(2)
            try:
                result = chat_function(user_input)
            except Exception as e:
                st.error(f"An error occurred: {str(e)}")
                return

        # Display bot's response
        conversation.append(f"Bot: {result}")

        # Save conversation history in session state
        st.session_state.conversation = conversation

        # Display conversation history
        chat_messages = st.empty()
        chat_messages.text("\n".join(conversation))

if __name__ == "__main__":
    main()
