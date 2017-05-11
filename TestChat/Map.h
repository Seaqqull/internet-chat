class MyMap {
public:
	CString TitleMap;
	UINT IdMap;
public:
	MyMap(){
		TitleMap = "";
		IdMap = 0;
	}	
	MyMap(CString title, UINT id){
		TitleMap = title;
		IdMap = id;
	}
	MyMap(MyMap& incomingMap){
		(*this) = incomingMap;
	}
	MyMap& operator=(MyMap& incomingMap){
		this->TitleMap = incomingMap.TitleMap;
		this->IdMap = incomingMap.IdMap;
		return *this;
	}
	bool operator==(MyMap incomingMap){
		return (this->TitleMap == incomingMap.TitleMap && this->IdMap == incomingMap.IdMap) ? true : false;
	}
};

class ManageMyMap{
public:

	CList<MyMap, MyMap&> Maps;
public:
	ManageMyMap(){ ; }
	~ManageMyMap(){ this->Clear(); }

	MyMap &GetLast(){ return Maps.GetTail(); }

	POSITION GetHead(){
		return Maps.GetHeadPosition();
	}

	MyMap & GetNext(POSITION & p){
		return Maps.GetNext(p);
	}

	POSITION Get(CString incomingTitle){
		POSITION pos = Maps.GetHeadPosition();
		while (pos){
			POSITION pos1 = pos;
			MyMap tempMap = Maps.GetNext(pos);
			if (tempMap.TitleMap == incomingTitle) return pos1;
		}
		return 0;
	}

	POSITION Get(UINT idMap){
		POSITION pos = Maps.GetHeadPosition();
		while (pos){
			POSITION pos1 = pos;
			MyMap tempMap = Maps.GetNext(pos);
			if (tempMap.IdMap == idMap) return pos1;
		}
		return 0;
	}

	POSITION Get(MyMap incomingMap){
		POSITION pos = Maps.GetHeadPosition();
		while (pos){
			POSITION pos1 = pos;
			MyMap tempMap = Maps.GetNext(pos);
			if (tempMap == incomingMap) return pos1;
		}
		return 0;
	}

	MyMap & GetMap(POSITION p){ return Maps.GetAt(p); }

	CString GetTitleMap(UINT idMap){
		POSITION pos = Get(idMap);		
		return GetMap(pos).TitleMap;
	}

	bool IsMap(CString titleMap){
		POSITION pos = Get(titleMap);		
		return (pos != NULL) ? false : true;
	}
	int GetIdMap(CString titleMap){
		POSITION pos = Get(titleMap);
		return (pos != NULL) ? GetMap(pos).IdMap : -1;
	}

	bool Add(CString Title, UINT id){
		bool IsSimilar = false;
		POSITION pos = Maps.GetHeadPosition();
		while (pos){
			POSITION pos1 = pos;
			MyMap tempMap = Maps.GetNext(pos);
			if (tempMap.TitleMap == Title) {
				IsSimilar = true;
				break;
			}
		}
		if (!IsSimilar)
			Maps.AddTail(MyMap(Title, id));
		
		return (!IsSimilar) ? true : false;
	}

	void Remove(MyMap incomingMap){
		POSITION pos = Get(incomingMap);
		if (pos) Maps.RemoveAt(pos);
	}
	void Remove(CString incomingStr){
		POSITION searchPos = this->Get(incomingStr);
		this->Remove(this->GetMap(searchPos));
	}

	void Clear(){
		Maps.RemoveAll();
	}
};