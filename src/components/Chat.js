import React, {
	Component,
	PropTypes,
	StyleSheet,
	Text,
	ScrollView,
	View
} from 'react-native';

import Bubble from './Bubble';
import AnimatedTextInput from './AnimatedTextInput';
import _ from 'lodash';

export default class Chat extends Component {

	static propTypes = {
		addMessage: PropTypes.func.isRequired,
		conversation: PropTypes.object.isRequired,
		conversationId: PropTypes.string.isRequired
	}

	// let's load in some bubbles
	constructor(props) {
		super(props)

		console.log('chat',this.props);
	}

	render() {
		return(
			<View>
				<ScrollView>
					{
						_.map(this.props.messages, (message) => {
							return <Bubble 	key={_.uniqueId()}
											text={message.body}
										   	position={message.position}/>
						})
					}
				</ScrollView>
				<AnimatedTextInput
						addMessage={this.props.addMessage}
						conversation={this.props.conversation}/>
			</View>
		)
	}
}