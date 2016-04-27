import React, { Component, PropTypes, View } from 'react-native'
import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';
import _ from 'lodash';

import ChatCell from '../components/ChatCell';

const mapStateToProps = state => {
   return {
      conversation: state.conversation,
      messages: state.message
   };
};

class ChatList extends Component {
	render() {
		return (
			<View>
				{
					/* 
						wrap in a touchableopacity to get the onTouch event
						and call and call action to route with the conversation key
					*/
					_.map(this.lastMessage(), (message) => {
						console.log(message); // <- last message
						return <ChatCell lastMessage={message}/>
					})
				}
			</View>
		)
	}

	lastMessage() {

		lastMsgKeys = _.map(this.props.conversation, (conv) => { 
			return conv.messages.pop()
		})

		lastMsges = _.map(lastMsgKeys, (key) => {
			return this.props.messages[key]
		})

		console.log(lastMsges);

		return lastMsges
	}

	conversationKey() {

	}
}

export default connect(mapStateToProps)(ChatList);