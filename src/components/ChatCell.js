import React, {
	Component,
	PropTypes,
	StyleSheet,
	Text,
	Dimensions,
	View
} from 'react-native';
import Immutable from 'immutable';

// TODO: optimise the preview of the last message (unneccerairy calls)
const ChatCell = ({
	lastMessage,
	messages
}) =>
	<View>
		<View style={styles.cell}>
			{console.log('lastMessage', lastMessage)}
			<View style={styles.image}>
				<Text style={styles.initial}>
					{ ('' + lastMessage.from).charAt(0) }
				</Text>
			</View>
			<View style={{ flexDirection: 'column', padding: 10 }}>
				<View style={{ flexDirection: 'row', marginTop: 10}}>
					<Text style={styles.name}>
						{lastMessage.from}
					</Text>
					<Text style={styles.day}>
						{lastMessage.timestamp}
					</Text>
				</View>
				<Text style={styles.msgPreview}>
					{(!lastMessage.body === "undefined" && lastMessage.body.length > 30) ? lastMessage.body.substring(0, 30) + '...' : lastMessage.body}
				</Text>
			</View>
		</View>
		<View style={styles.divider}/>
	</View>

ChatCell.propTypes = {
	lastMessage: React.PropTypes.object.isRequired
}

ChatCell.defaultProps = {
	...Component.defaultProps,
	// TODO: move to real messages, replace the dummy data
	lastMessage: Immutable.Map  ({
								  from: 'Jakub',
								  message: 'xBand equalizes opportopportunityopportunityunity!',
								  time: 'Mon' // Will I get it parsed from Java already? need to display the day in shorthand
								})
}

const styles = StyleSheet.create({
	cell: {
		flexDirection: 'row',
	},
	image: {
		height: Dimensions.get('window').width / 6,
		width: Dimensions.get('window').width / 6,
		borderRadius: Dimensions.get('window').width / 12,
		borderColor: '#757575',
		borderWidth: 1,
		alignSelf: 'flex-start',
		backgroundColor: '#F5F5F5',
		justifyContent: 'center',
		margin: 10,
	},
	initial: {
		backgroundColor: 'transparent',
		color: '#757575',
		fontSize: 40,
		textAlign: 'center',
		textAlignVertical: 'center',
		fontWeight: '200'
	},
	preview: {
		flexDirection: 'column'
	},
	name: {
		flex: 1,
		color: '#757575',
		fontSize: 20,
		textAlign: 'left',
	},
	day: {
		flex: 1,
		color: '#9E9E9E',
		fontSize: 18,
		textAlign: 'right',
	},
	msgPreview: {
		flex: 1,
		color: '#9E9E9E',
		fontSize: 18,
		textAlign: 'right',
		textAlignVertical: 'center',
		marginTop: 2
	},
	divider: {
		flex: 1,
		backgroundColor: '#757575',
		height: 1,
		alignSelf: 'center',
		width: Dimensions.get('window').width - 50
	}
})

export default ChatCell