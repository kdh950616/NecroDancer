#include "stdafx.h"
#include "soundManager.h"


soundManager::soundManager()
	:_system(NULL),
	_channel(NULL),
	_sound(NULL)
{
}


soundManager::~soundManager()
{

}

void soundManager::playZone(string keyName, float volume)
{
	int count = 0;
	int count_BGM, count_shop;														//배경, 상점주인용 카운트

	string keyName_shop = keyName + "_shopkeeper";									//상점주인용 키값을 위해 쓰인다.
	arrSoundsIter iter_BGM = _mTotalSounds.begin();									//배경음악용 iter
	arrSoundsIter iter_shop = _mTotalSounds.begin();								//상점주인용 iter
	arrSoundsIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			iter_BGM = iter;
			count_BGM = count;
		}
		else if (keyName_shop == iter->first)
		{

			iter_shop = iter;
			count_shop = count;
		}
	}
	_system->playSound(FMOD_CHANNEL_FREE, *iter_BGM->second, false, &_channel[count_BGM]);
	_channel[count_BGM]->setVolume(volume);

	_system->playSound(FMOD_CHANNEL_FREE, *iter_shop->second, false, &_channel[count_shop]);
	_channel[count_shop]->setVolume(volume);
}

void soundManager::pauseZone(string keyName)
{
	int count = 0;
	int count_BGM, count_shop;												//배경, 상점주인용 카운트

	string keyName_shop = keyName + "_shopkeeper";							//상점주인용 키값을 위해 쓰인다.
	arrSoundsIter iter_BGM = _mTotalSounds.begin();							//배경음악
	arrSoundsIter iter_shop = _mTotalSounds.begin();						//상점주인
	arrSoundsIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			iter_BGM = iter;
			count_BGM = count;
		}
		else if (keyName_shop == iter->first)
		{
			iter_shop = iter;
			count_shop = count;
		}
	}
	_channel[count_BGM]->setPaused(true);
	_channel[count_shop]->setPaused(true);

}

void soundManager::resumeZone(string keyName, float volume)
{
	int count = 0;
	int count_BGM, count_shop;												//배경, 상점주인용 카운트

	string keyName_shop = keyName + "_shopkeeper";							//상점주인용 키값을 위해 쓰인다.
	arrSoundsIter iter_BGM = _mTotalSounds.begin();							//배경음악
	arrSoundsIter iter_shop = _mTotalSounds.begin();						//상점주인
	arrSoundsIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			iter_BGM = iter;
			count_BGM = count;
		}
		else if (keyName_shop == iter->first)
		{
			iter_shop = iter;
			count_shop = count;
		}
	}

	_channel[count_BGM]->setPaused(false);
	_channel[count_shop]->setPaused(false);
}

void soundManager::ShopVolume(string keyName, float volume)
{
	int count = 0;
	int count_BGM, count_shop;												//배경, 상점주인용 카운트
	_shopVolume = volume;

	string keyName_shop = keyName + "_shopkeeper";							//상점주인용 키값을 위해 쓰인다.
	arrSoundsIter iter_BGM = _mTotalSounds.begin();							//배경음악
	arrSoundsIter iter_shop = _mTotalSounds.begin();						//상점주인
	arrSoundsIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			iter_BGM = iter;
			count_BGM = count;
		}
		else if (keyName_shop == iter->first)
		{
			iter_shop = iter;
			count_shop = count;
		}
	}
	_channel[count_shop]->setVolume(volume);

}

void soundManager::playBossZone(string keyName, float volume)
{
	int count = 0;
	int count_bass, count_drum, count_horn, count_keytar, count_strings;

	string keyName_bass = keyName + "_bass";
	string keyName_drum = keyName + "_drum";
	string keyName_horn = keyName + "_horn";
	string keyName_keytar = keyName + "_keytar";
	string keyName_strings = keyName + "_strings";
	arrSoundsIter iter_bass = _mTotalSounds.begin();
	arrSoundsIter iter_drum = _mTotalSounds.begin();
	arrSoundsIter iter_horn = _mTotalSounds.begin();
	arrSoundsIter iter_keytar = _mTotalSounds.begin();
	arrSoundsIter iter_strings = _mTotalSounds.begin();
	arrSoundsIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName_bass == iter->first)
		{

			iter_bass = iter;
			count_bass = count;
		}
		else if (keyName_drum == iter->first)
		{

			iter_drum = iter;
			count_drum = count;
		}
		else if (keyName_horn == iter->first)
		{

			iter_horn = iter;
			count_horn = count;
		}
		else if (keyName_keytar == iter->first)
		{

			iter_keytar = iter;
			count_keytar = count;
		}
		else if (keyName_strings == iter->first)
		{

			iter_strings = iter;
			count_strings = count;
		}
	}

	_system->playSound(FMOD_CHANNEL_FREE, *iter_bass->second, false, &_channel[count_bass]);
	_channel[count_bass]->setVolume(volume);

	_system->playSound(FMOD_CHANNEL_FREE, *iter_drum->second, false, &_channel[count_drum]);
	_channel[count_drum]->setVolume(volume);

	_system->playSound(FMOD_CHANNEL_FREE, *iter_horn->second, false, &_channel[count_horn]);
	_channel[count_horn]->setVolume(volume);

	_system->playSound(FMOD_CHANNEL_FREE, *iter_keytar->second, false, &_channel[count_keytar]);
	_channel[count_keytar]->setVolume(volume);

	_system->playSound(FMOD_CHANNEL_FREE, *iter_strings->second, false, &_channel[count_strings]);
	_channel[count_strings]->setVolume(volume);
}

unsigned int soundManager::getlength(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	unsigned int length;

	for (; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_sound[count]->getLength(&length, FMOD_TIMEUNIT_MS);
			return length;
		}
	}
}

void soundManager::loadBeat(const char * fileName, string keyName)
{
	_vBeat.clear();																//비운다. 비트벡터. 깔끔히

	vector<string> vTemp;														//임시로 비트를 담을 벡터
	vTemp = TXTDATA->txtLoad(fileName);											//파일을 로드한다. 로드하면서 비트는 구분자에 의해 쪼개진다.
	int size = vTemp.size();													//임시벡터만큼 사이즈로 비트벡터에 넣을 예정
	tagBeat	note;																	//stoi에 필요할 변수

	if (_mBeat.find(keyName) == _mBeat.end())									//만약 키값이 맵에 없을경우(중복X)
	{
		for (int i = 0; i < size; i++)											//임시벡터만큼 for문 돌린다.
		{
			note.beat = stoi(vTemp[i]);											//임시벡터에 있는 string을 int로 변환해서 note에 넣어준다.

			_vBeat.push_back(note);												//비트벡터에 note에 들어있는 값을 넣어준다.
		}
		_mBeat.insert(make_pair(keyName, _vBeat));								//맵에 키값과 비트벡터 넣어준다.

	}
	else																		//키값이 맵에 존재한다면(중복O)
	{
		for (_miBeat = _mBeat.begin(); _miBeat != _mBeat.end(); _miBeat++)		//iter로 for문 돌린다. (중복된 키값 찾기위해)
		{
			if (_miBeat->first == keyName)										//키값 찾았으면
			{
				_vBeat = _miBeat->second;										//맵에 저장된 값을 넣어준다.
				break;
			}

		}
	}
}

void soundManager::setPitch(float pitch)
{
	_system->getMasterChannelGroup(&_bgmGroup);
	_system->getMasterChannelGroup(&_effectGroup);
	_bgmGroup->setPitch(pitch);
	_effectGroup->setPitch(pitch);
}

int soundManager::getLength(string keyName)
{
	int count = 0;
	int count_BGM, count_shop;														//배경, 상점주인용 카운트
	UINT start = 0;

	string keyName_shop = keyName + "_shopkeeper";									//상점주인용 키값을 위해 쓰인다.
	arrSoundsIter iter_BGM = _mTotalSounds.begin();									//배경음악용 iter
	arrSoundsIter iter_shop = _mTotalSounds.begin();								//상점주인용 iter
	arrSoundsIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			count_BGM = count;
			_sound[count_BGM]->getLength(&start, FMOD_TIMEUNIT_MS);
			return start;
		}
		else if (keyName_shop == iter->first)
		{
			count_shop = count;
			_sound[count_shop]->getLength(&start, FMOD_TIMEUNIT_MS);
			return start;
		}
	}
}

void soundManager::setZoneVolume(string keyName,float volume)
{
	int count = 0;
	int count_BGM, count_shop;														//배경, 상점주인용 카운트

	string keyName_shop = keyName + "_shopkeeper";									//상점주인용 키값을 위해 쓰인다.
	arrSoundsIter iter_BGM = _mTotalSounds.begin();									//배경음악용 iter
	arrSoundsIter iter_shop = _mTotalSounds.begin();								//상점주인용 iter
	arrSoundsIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			iter_BGM = iter;
			count_BGM = count;
		}
		else if (keyName_shop == iter->first)
		{

			iter_shop = iter;
			count_shop = count;
		}
	}

	_channel[count_BGM]->setVolume(volume);
	_channel[count_shop]->setVolume(volume);
}

int soundManager::getPosition(string keyName)
{
	int count = 0;
	int count_BGM, count_shop;														//배경, 상점주인용 카운트
	UINT now = 0;

	string keyName_shop = keyName + "_shopkeeper";									//상점주인용 키값을 위해 쓰인다.
	arrSoundsIter iter_BGM = _mTotalSounds.begin();									//배경음악용 iter
	arrSoundsIter iter_shop = _mTotalSounds.begin();								//상점주인용 iter
	arrSoundsIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			count_BGM = count;
			_channel[count_BGM]->getPosition(&now, FMOD_TIMEUNIT_MS);
			return now;
		}
		else if (keyName_shop == iter->first)
		{
			count_shop = count;
			_channel[count_shop]->getPosition(&now, FMOD_TIMEUNIT_MS);
			return now;
		}
	}
}

void soundManager::getSingShopkeeper(string keyName)
{
	int size = 64;

	_specLeft = new float[size];
	_specRight = new float[size];
	_spec = new float[size];

	int count = 0;
	int count_BGM, count_shop;														//배경, 상점주인용 카운트
	UINT now = 0;

	string keyName_shop = keyName + "_shopkeeper";									//상점주인용 키값을 위해 쓰인다.
	arrSoundsIter iter_BGM = _mTotalSounds.begin();									//배경음악용 iter
	arrSoundsIter iter_shop = _mTotalSounds.begin();								//상점주인용 iter
	arrSoundsIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName_shop == iter->first)
		{
			count_shop = count;
			_channel[count_shop]->getSpectrum(_specLeft, size, 0, FMOD_DSP_FFT_WINDOW_RECT);
			_channel[count_shop]->getSpectrum(_specRight, size, 1, FMOD_DSP_FFT_WINDOW_RECT);
			break;
		}
	}

	for (int i = 0; i < size; i++)
	{
		_spec[i] = (_specLeft[i] + _specRight[i]) / 2;
	}

	auto maxIterator = std::max_element(&_spec[0], &_spec[size]);
	float maxVolume = *maxIterator;

	if (maxVolume != 0)
		std::transform(&_spec[0], &_spec[size], &_spec[0], [maxVolume](float dB) -> float { return dB / maxVolume; });

}

HRESULT soundManager::init()
{
	//FMOD 사운드 엔진을 사용하겠다고 선언
	System_Create(&_system);

	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, NULL);

	_sound = new Sound*[TOTALSOUNDBUFFER];
	_channel = new Channel*[TOTALSOUNDBUFFER];

	memset(_sound, 0, sizeof(Sound*) * (TOTALSOUNDBUFFER));
	memset(_channel, 0, sizeof(Channel*) * (TOTALSOUNDBUFFER));

	// 사운드 그룹 생성
	_system->createSoundGroup("bgmSound", &_bgmSoundGroup);
	_system->createSoundGroup("effectSound", &_effectSoundGroup);

	// 채널 그룹 생성
	_system->createChannelGroup("bgm", &_bgmGroup);
	_system->createChannelGroup("effect", &_effectGroup);

	return S_OK;
}

void soundManager::release()
{
	//사운드 삭제
	if (_channel != NULL || _sound != NULL)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (_channel != NULL)
			{
				if (_channel[i]) _channel[i]->stop();
			}

			if (_sound != NULL)
			{
				if (_sound != NULL) _sound[i]->release();
			}
		}
	}

	//메모리 지우기
	SAFE_DELETE_ARRAY(_channel);
	SAFE_DELETE_ARRAY(_sound);

	SAFE_RELEASE(_effectGroup);
	SAFE_RELEASE(_bgmGroup);

	SAFE_RELEASE(_bgmSoundGroup);
	SAFE_RELEASE(_effectSoundGroup);

	_effectGroup = nullptr;
	_bgmGroup = nullptr;
	_bgmSoundGroup = nullptr;
	_effectSoundGroup = nullptr;

	delete[] _spec;
	delete[] _specLeft;
	delete[] _specRight;

	//시스템 닫기 
	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}
}

void soundManager::update()
{
	//사운드 System 계속적으로 업데이트
	_system->update();

	//볼륨이 바뀌거나 
	//재생이 끝난 사운드를 채널에서 빼내는등의 다양한
	//작업을 자동으로 해준다
}


void soundManager::addSound(string keyName, string soundName, bool bgm, bool loop)
{
	// 사운드는 TOTALSOUNDBUFFER개 까지만 추가 할 수 있다.
	if (TOTALSOUNDBUFFER <= _mTotalSounds.size())
		return;

	// 루프재생?
	FMOD_MODE mode = NULL;
	if (loop)
		mode = FMOD_LOOP_NORMAL;
	else
		mode = FMOD_DEFAULT;

	// 비어있는 사운드칸에 새로운 사운드를 할당
	int useSoundIdx = static_cast<int>(_mTotalSounds.size());
	if (bgm)
	{
		_system->createStream(soundName.c_str(), mode, NULL, &_sound[useSoundIdx]);
		_sound[useSoundIdx]->setSoundGroup(_bgmSoundGroup);
	}
	else
	{
		_system->createSound(soundName.c_str(), mode, NULL, &_sound[useSoundIdx]);
		_sound[useSoundIdx]->setSoundGroup(_effectSoundGroup);
	}

	_mTotalSounds.insert(make_pair(keyName, &_sound[useSoundIdx]));
}

void soundManager::play(string keyName, float volume)// 0.0 ~ 1.0f -> 0 ~ 255
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	for (iter; iter != _mTotalSounds.end(); ++iter, ++count)
	{
		Sound* sound = *iter->second;
		SoundGroup* soundGroup = nullptr;

		if (keyName == iter->first)
		{
			// 어느 사운드 그룹에 속해있는지 확인
			sound->getSoundGroup(&soundGroup);

			if (soundGroup == _bgmSoundGroup)	// bgm soundgroup
			{
				_bgmSoundGroup->stop(); // 플레이중인 bgm 정지
				_channel[count]->setChannelGroup(_bgmGroup);
			}
			else								// effect soundgroup
				_channel[count]->setChannelGroup(_effectGroup);

			_system->playSound(FMOD_CHANNEL_FREE, sound, false, &_channel[count]);
			_channel[count]->setVolume(volume);

			break;
		}
	}
}

void soundManager::playEff(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	for (iter; iter != _mTotalSounds.end(); ++iter, ++count)
	{
		Sound* sound = *iter->second;
		SoundGroup* soundGroup = nullptr;

		if (keyName == iter->first)
		{
			// 어느 사운드 그룹에 속해있는지 확인
			sound->getSoundGroup(&soundGroup);

			if (soundGroup == _bgmSoundGroup)	// bgm soundgroup
			{
				_bgmSoundGroup->stop(); // 플레이중인 bgm 정지
				_channel[count]->setChannelGroup(_bgmGroup);
			}
			else								// effect soundgroup
				_channel[count]->setChannelGroup(_effectGroup);

			_system->playSound(FMOD_CHANNEL_REUSE, sound, false, &_channel[count]);
			_channel[count]->setVolume(_effectVol);

			break;
		}
	}
}

void soundManager::stop(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();
			break;
		}
	}
}

void soundManager::pause(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(true);
			break;
		}
	}
}

void soundManager::resume(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(false);
			break;
		}
	}
}


bool soundManager::isPlaySound(string keyName)
{
	bool isPlay = false;
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

bool soundManager::isPauseSound(string keyName)
{
	bool isPause = false;
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->getPaused(&isPause);
			break;
		}
	}

	return isPause;
}

void soundManager::setEffectVolume(float volume)
{
	if (volume < 0.f)
		volume = 0.f;
	else if (1.0f < volume)
		volume = 1.0f;

	_effectGroup->setVolume(volume);

	cout << "effect volume : " << volume << endl;
}

void soundManager::setBgmVolume(float volume)
{
	if (volume < 0.f)
		volume = 0.f;
	else if (1.0f < volume)
		volume = 1.0f;

	_bgmGroup->setVolume(volume);

	cout << "bgm volume : " << volume << endl;
}

void soundManager::setAllSoundVolume(float volume)
{
	_bgmGroup->setVolume(volume);
	_effectGroup->setVolume(volume);
}

void soundManager::setEffectPause()
{
	_effectGroup->setPaused(true);
}

void soundManager::setBgmPause()
{
	_bgmGroup->setPaused(true);
}

void soundManager::setAllSoundPause()
{
	_bgmGroup->setPaused(true);
	_effectGroup->setPaused(true);
}

void soundManager::setEffectResume()
{
	_effectGroup->setPaused(false);
}

void soundManager::setBgmResume()
{
	_bgmGroup->setPaused(false);
}

void soundManager::setAllSoundResume()
{
	_bgmGroup->setPaused(false);
	_effectGroup->setPaused(false);
}
