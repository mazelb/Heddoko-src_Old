using UnityEngine;
using System.Collections;

public class NodKeyboardButton : MonoBehaviour
{
	public enum FunctionKey { Shift, BackSpace, Symbols, Enter };

	public bool isAlphaNumeric = true;
	public char letter;
	public FunctionKey functionKey;
}
