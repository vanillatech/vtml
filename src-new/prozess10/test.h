#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace prozess10 {

	/// <summary>
	/// Zusammenfassung f�r test
	///
	/// Warnung: Wenn Sie den Namen dieser Klasse �ndern, m�ssen Sie auch
	///          die Ressourcendateiname-Eigenschaft f�r das Tool zur Kompilierung verwalteter Ressourcen �ndern,
	///          das allen RESX-Dateien zugewiesen ist, von denen diese Klasse abh�ngt.
	///          Anderenfalls k�nnen die Designer nicht korrekt mit den lokalisierten Ressourcen
	///          arbeiten, die diesem Formular zugewiesen sind.
	/// </summary>
	public ref class test : public System::Windows::Forms::Form
	{
	public:
		test(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzuf�gen.
			//
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~test()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	protected: 

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode f�r die Designerunterst�tzung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor ge�ndert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(175, 40);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(97, 22);
			this->button1->TabIndex = 0;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// test
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 272);
			this->Controls->Add(this->button1);
			this->Name = L"test";
			this->Text = L"test";
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
