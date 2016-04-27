import React, {
	Component,
	PropTypes,
	StyleSheet,
	Dimensions,
	Text,
	View,
	ScrollView
} from 'react-native';

export default class ChatList extends Component {

	static propTypes = {
		conversations: PropTypes.Object.isRequired
	}

	// let's load in some bubbles

	constructor(props) {
		super(props)
		console.log('hey', this.props.addMessage);
	}

	render() {
		// this should iterate over the prop and pass props down to Bubble
		return(
			
			<View>
				<Bubble text='haha' position='right'/>
				<Bubble text='ahaha' position='left'/>
				<Bubble text="that's funny" position='right'/>
				<Bubble text='real funny man' position='right'/>
				<AnimatedTextInput
					text={''}
					updateText={this.props.updateText}
					addMessage={this.props.addMessage}/>
			</View>
		)
	}
}