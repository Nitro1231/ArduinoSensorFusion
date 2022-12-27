using System.Collections;
using System.Collections.Generic;
using UnityEngine;


using extOSC;


public class GameObjectMovement : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        Debug.Log("Test");
        var receiver = gameObject.AddComponent<OSCReceiver>();
        receiver.LocalPort = 8080;
        receiver.Bind("/euler_unity", onQuatReceived);
    }

    // Update is called once per frame
    void Update()
    {

    }

    protected void onQuatReceived(OSCMessage quat)
    {
        // Debug.Log("!!");
        // Debug.Log(message);
        transform.rotation = Quaternion.Euler(quat.Values[2].FloatValue, -quat.Values[3].FloatValue, -quat.Values[1].FloatValue);
    }
}
