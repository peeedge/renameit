#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "../resource.h"
#include "RenamingList.h"

/** CProgressDlg dialog: Display a progression.
 * Use EnableCancel(true) before DoModal() to allow operation cancelling.
 * Other methods should be thread-safe: You can call them before, during or even after DoModal().
 *
 * Use SetTitle("My Progress") and SetCaption("Operation $(Done) of $(Total)...") to change
 * the title and the displayed text.
 *
 * During DoModal(), call the SetProgress() from another thread to update the current progress.
 *
 * Call Done() when the operation is complete, and the dialog will return IDOK.
 * If during the operation, if the user pressed the Cancel button (when enabled), it returns IDCANCEL.
 */
class CProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProgressDlg();

// Pre-display attributes
	// Change the dialog title.
	void SetTitle(const CString& strValue) {
		// If the new title is different update the title next time.
		if (m_strDialogTitle != strValue)
		{
			m_strDialogTitle = strValue;
			m_bRefreshEverything = true;
		}
	}
	void SetTitle(UINT nStringID) {
		CString strNewTitle;
		strNewTitle.LoadString(nStringID);
		SetTitle(strNewTitle);
	}

	/**
	 * Change the progress caption.
	 * @param strValue New caption where "$(Done)", "$(Total)" and "$(Percents)" are replaced by their values.
	 */
	void SetCaption(const CString& strValue) {
		m_strProgressCaption = strValue;
	}
	void SetCaption(UINT nStringID) {
		m_strProgressCaption.LoadString(nStringID);
	}

	// Enable or disable the user cancelling possibility.
	void EnableCancel(bool bEnable=true) {
		if (bEnable != m_bEnableCancel)
		{
			m_bEnableCancel = bEnable;
			m_bRefreshEverything = true;
		}
	}

	void Done() {
		m_bDone = true;
	}

// Operations (can always be used)
	/**
	 * Set the current progress.
	 * @param nStage The current renaming stage.
	 * @param nDone Number of elements done in this stage over [0, nTotal].
	 * @param nTotal Total number of elements in this stage.
	 */
	void SetProgress(CRenamingList::EStage nStage, unsigned nDone, unsigned nTotal) {
		// If we change to a greater stage...
		if (m_nStage != nStage)
		{
			// Update the current stage.
			m_nStage = nStage;
			m_bRefreshEverything = true;
		}

		if (m_nTotal != nTotal)
		{
			ASSERT(nTotal > 0);
			m_nTotal = nTotal;
			m_bRefreshEverything = true;
		}

		m_nDone = nDone;
	}

// Dialog Data
private:
	enum { IDD = IDD_RENAMING_PROGRESS };

	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	// Update the current progress displayed from m_nDone and m_nTotal.
	void UpdateProgress();

	CRenamingList::EStage m_nStage;	// Current stage.
	unsigned m_nDone;	// Number of items completed in the current stage.
	unsigned m_nTotal;	// Total number of items in the current stage.
	bool m_bRefreshEverything;	// True everything in the windows should be updated.
	bool m_bDone;		// Set to true when the total operation is complete.

	CString m_strDialogTitle;
	CString m_strProgressCaption;
	bool m_bEnableCancel;
	CStatic m_ctlProgressStatic;
	CProgressCtrl m_ctlProgress[CRenamingList::stageCount];
public:
	virtual INT_PTR DoModal();
};