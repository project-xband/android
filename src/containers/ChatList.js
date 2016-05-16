import React, { Component, PropTypes, View, Text, ScrollView, TouchableOpacity, TouchableHighlight } from 'react-native'
import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';
import _ from 'lodash';

import ChatCell from '../components/ChatCell';
import { Actions } from 'react-native-router-flux'

const mapStateToProps = state => {
   return {
   	  isLoaded: state.storage.isLoaded,
      conversation: state.conversation,
      messages: state.message,
   };
};

/* 
	TODO

	something in this file or it's children is fucking the store up. 

	if ChatCell fails to load the store loads itself fine
	try commenting the _.map({}) out and see for yourself
*/

class ChatList extends Component {
	render() {
		if (!this.props.isLoaded) {
			console.log('is not loaded')
			return (
				<View>
					<Text>
						Loading...
					</Text>
				</View>
			)
		}
		console.log('is loaded')
		
		// return ( <Text style={{marginTop: 100}}>Hey</Text>)
		return (
			<ScrollView style={{ marginTop: 60 }}>
				{
					_.map(this.lastMessage(), (keys) => {
						const conversationKey = keys.conversationKey
						const message 		= this.props.messages[keys.messageKey]

						return (
							<TouchableOpacity onPress={ () => { Actions.ChatContainer({ conversationKey: conversationKey })}}
											  key={_.uniqueId()}>
								<ChatCell lastMessage={ message }
										  key={_.uniqueId()}/>
							</TouchableOpacity>
						)
					})
				}
			</ScrollView>
		)
	}

	lastMessage() {
		lastMsgKeys = _.map(this.props.conversation, (conv, key) => {
			return {
				conversationKey: key,
				messageKey: conv.messages.slice(-1)[0]
			}
		})
		return lastMsgKeys
	}
}

export default connect(mapStateToProps)(ChatList);