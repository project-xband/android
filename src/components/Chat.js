import React, {
	Component,
	PropTypes,
	StyleSheet,
	Text,
	View,
  DeviceEventEmitter
} from 'react-native';
import Immutable from 'immutable';
import Bubble from './Bubble';
import AnimatedTextInput from './AnimatedTextInput';
import SendModuleAndroid from './SendModuleAndroid';

export default class Chat extends Component {

	static propTypes = {
		// unsure how to structure this, should use the redux store directly?
		addMessage: PropTypes.func.isRequired,
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
				<AnimatedTextInput addMessage={this.props.addMessage}/>
			</View>
		)
	}
}