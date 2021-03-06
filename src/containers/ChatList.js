
/*
	Note that this component can probably
	be broken up in to a Container Component
	'ChatListContainer' and a Presentational
	Component 'ChatList' - in current state
	can cause confusion in what it's function
	really is #futurerefactor
*/

import React, {
	Component,
	PropTypes,
	View,
	Text,
	ScrollView,
	TouchableOpacity,
	TouchableHighlight
} from 'react-native'

/*
  ^ there is a new format they said (new RN 0.25.1)
  where the import is like -->

  ````
    import React, { Component } from 'react'
    import {
      PropTypes,
      StyleSheet,
      Dimensions,
      Text,
      TextInput,
      View,
      TouchableHighlight
    } from 'react-native'
  ````

  (https://github.com/facebook/react-native/releases/tag/v0.25.1)
*/

import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import _ from 'lodash'

import ChatCell from '../components/ChatCell'
import { Actions } from 'react-native-router-flux'

const mapStateToProps = state => {
 return {
	  isLoaded: state.storage.isLoaded,
	  conversation: state.conversation,
	  messages: state.message
   }
}

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
			<ScrollView style = {{ marginTop: 60 }}>
				{
					_.map(this.lastMessage(), (keys) => {
						const conversationKey = keys.conversationKey
						const message = this.props.messages[keys.messageKey]

						return (
							<TouchableOpacity
								onPress={ () => { Actions.ChatContainer({ conversationKey: conversationKey }) } }
								key={ _.uniqueId() }>
								<ChatCell
									lastMessage = { message }
									key={ _.uniqueId() }/>
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

export default connect(mapStateToProps)(ChatList)
