/************************************************************************/
/* 创建一个window基类,代号为THWindow									*/
/*谨记，加virtual的目的是为了多态										*/
/************************************************************************/
#include <windows.h>
class THWindow
{
protected:
	HINSTANCE hInst;
	HWND hParent;
	HWND hSelf;
public:
	THWindow()
		: hInst(NULL), hParent(NULL), hSelf(NULL)
	{
	
	}

	virtual ~THWindow() 
	{
	
	}
	
	virtual void init(HINSTANCE inst, HWND parent)
	{
		hInst = inst;
		hParent = parent;
	}
	
	virtual void destroy() = 0;
	
	virtual void display(bool toShow = true) const
	{
		::ShowWindow(hSelf, toShow ? SW_SHOW : SW_HIDE);
	}

	virtual void reSizeTo(RECT &rt)
	{
		::MoveWindow(hSelf, rt.left, rt.top, rt.right, rt.bottom, TRUE);
		redraw();
	}

	virtual void redraw() const
	{
		::InvalidateRect(hSelf, NULL, TRUE);
		::UpdateWindow(hSelf);
	}

	virtual void getClientRect(RECT &rt) const
	{
		::GetClientRect(hSelf, rt);
	}

	virtual int getWidth() const 
	{
		RECT rt;
		getClientRect(rt);
		return (rt.right - rt.left);
	}

	virtual int getHeight() const
	{
		RECT rt;
		getClientRect(rt);
		return (rt.bottom - rt.top);
	}

	virtual bool isVisible() const
	{
		return bool(::IsWindowVisible(hSelf));
	}

	HWND getHSelf() const
	{
		if(hSelf == NULL)
		{
			::MessageBox(NULL, "hSelf == NULL", "class window", MB_OK);
			throw int(999);
		}
		return hSelf;
	}
	
	virtual void getFocus() const
	{
		::SetFocus(hSelf);
	}

	HINSTANCE getInstance() const
	{
		if(!hInst){	// !hInst 意思是：如果没有hInst
			::MessageBox(NULL, "hInst == NULL", "class window", MB_OK);
			throw int(1999);
		}
		return hInst;
	}
};