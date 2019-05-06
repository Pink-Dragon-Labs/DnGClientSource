#ifndef _TOON_SETTINGS_HPP
#define _TOON_SETTINGS_HPP

class CToonSettings : public CObject
{
DECLARE_SERIAL( CToonSettings );

protected:
	CString	m_sToonName;
	CString	m_sServer;

	//		variables that need to be saved

	//		Channel 
	int		m_nChannel_Number;					//	-1 means no channel
	int		m_nChannel_AttemptJoinNumber;		//	-1 means no channel
	CString	m_sChannel_Title;
	CString	m_sChannel_Topic;
	CString	m_sChannel_Password;
	BOOL	m_bChannel_Leader;

	//		Chat
	BOOL	m_bChat_Busy;
	BOOL	m_bChat_Profanity;
	int		m_nChat_TextColor;

	//		Group
	BOOL	m_bGroup_Open;

	//		Combat
	int		m_nCombat_Speed;

	BOOL	m_bAccept;
	BOOL	m_bLagDisplay;
	BOOL	m_bTabDisplay;
    BOOL    m_bLogOff;

	//		Sound
	BOOL	m_bMidiOn;
	int		m_nMidiVolume;
	BOOL	m_bSfxOn;
	int		m_nSfxVolume;

	//		Video
	BOOL	m_bFullscreen;

	//		Alt Enter
	BOOL	m_bAltEnter;

	//		Rights
	int		m_nRights;

public:
	CToonSettings( int nRights = 0 );

	//		Load a specific toon from the account
	void	load( const char* sToonName, const char* sServer );

	//		Function to save all values to disk.
	virtual void Serialize( CArchive& ar );

	BOOL	getChat_Busy() { return m_bChat_Busy; }
	void	setChat_Busy( BOOL bBusy );

	BOOL	getChat_Profanity() { return m_bChat_Profanity; }
	void	setChat_Profanity( BOOL bProfanity );

	int		getChat_TextColor() { return m_nChat_TextColor; }
	void	setChat_TextColor( int nTextColor );
	BOOL	validChat_TextColor();

	BOOL	get_AutoAccept() { return m_bAccept; }
	void	set_AutoAccept( BOOL bAccept );

	BOOL	get_LagDisplay() { return m_bLagDisplay; }
	void	set_LagDisplay( BOOL bLag );

	BOOL	get_AltEnter() { return m_bAltEnter; }
	void	set_AltEnter( BOOL bLag );

	BOOL	get_TabDisplay() { return m_bTabDisplay; }
	void	set_TabDisplay( BOOL bTab );

	int		getCombat_Speed() { return m_nCombat_Speed; }
	void	setCombat_Speed( int nSpeed );

	//		Save the current settings.
	void	Save();

	void	LeaveChannel();
	void	SetChannelInfo( int nChannel, char* pTitle = NULL, char* pTopic = NULL, char* pPassword = NULL );

	int		getChannel_Number() { return m_nChannel_Number; }
	void	setChannel_Number( int nChannel );

	int		getChannel_AttemptJoin() { return m_nChannel_AttemptJoinNumber; }
	void	setChannel_AttemptJoin( int nChannel ) { m_nChannel_AttemptJoinNumber = nChannel; }

	CString	getChannel_Title() {return m_sChannel_Title; }
	void	setChannel_Title( char* pTitle );

	CString	getChannel_Topic() {return m_sChannel_Topic; }
	void	setChannel_Topic( char* pTopic );
	
	CString	getChannel_Password() {return m_sChannel_Password; }
	void	setChannel_Password( char* pPassword );

	bool	getChannel_Leader() {return m_bChannel_Leader; }
	void	setChannel_Leader( bool bLeader );

	BOOL	getGroup_Open() { return m_bGroup_Open; }
	void	setGroup_Open( BOOL bOpen );

	BOOL	getLogOff() { return m_bLogOff; }
    void	SetLogOff( BOOL bLog ) { m_bLogOff = bLog; }

	BOOL	getSound_MidiOn() { return m_bMidiOn; }
    void	setSound_MidiOn( BOOL bOn );

	int		getSound_MidiVolume() { return m_nMidiVolume; }
	void	setSound_MidiVolume( int nVolume );

	BOOL	getSound_SfxOn() { return m_bSfxOn; }
    void	setSound_SfxOn( BOOL bOn );

	int		getSound_SfxVolume() { return m_nSfxVolume; }
	void	setSound_SfxVolume( int nVolume );

	BOOL	getVideo_Fullscreen() { return m_bFullscreen; }
    void	setVideo_Fullscreen( BOOL bFullscreen );

	BOOL	getRights( int nMask ) { return ( m_nRights & nMask ); }
	void	setRights( int nMask );

	enum	ToonRights {
			trImplementor	=	0x00000001,
			trModerator		=	0x00000002,
			trGuide			=	0x00000004,
			trProphet		=	0x00000008,
			trCS			=	0x00000010,
			trEvent			=	0x00000020,
			trPublicRelations	=	0x00000040,
			trBuyFromStore	=	0x04000003,
			trDisable       =   0x00200000,
	};
};

extern CToonSettings* g_pToonSettings;
#endif
