using System.Collections;
using System.Collections.Generic;
using UnityEngine;


using extOSC;


public class FlightMovement : MonoBehaviour
{
    // Start is called before the first frame update

    public Rigidbody rb;
    public float speed = 10.0f;

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
        rb.velocity = -transform.right * speed;
    }

    protected void onQuatReceived(OSCMessage quat)
    {
        // Debug.Log("!!");
        // Debug.Log(message);
        transform.rotation = Quaternion.Euler(quat.Values[2].FloatValue, -quat.Values[3].FloatValue, -quat.Values[1].FloatValue);
    }
}
