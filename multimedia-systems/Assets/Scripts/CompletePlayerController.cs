using UnityEngine;
using System.Collections;

//Adding this allows us to access members of the UI namespace including Text.
using UnityEngine.UI;

public class CompletePlayerController : MonoBehaviour
{

    public float speed;             //variable to set the character's movement speed
    public Text countText;          //stores a reference to the text component of the UI that shows the number of collectibles collected
    public Text healthText;         //stores a reference to the text component of the UI showing the number of health points remaining
    public Text winText;            //stores a reference to the text component of the UI that displays the message 'You win'
    public Text loseText;           //stores a reference to the text component of the UI that displays the message 'You lose'

    public float health;            //float variable to set the character's health

    public GameObject player;       //creates a slot in the editor in which to place your character

    public AudioClip ding;          //stores a reference to the AudioClip component of the UI in which an audio track is contained
    public AudioSource coinDing;    //stores a reference to the AudioSource component of the UI in which an object of type AudioClip is deposited

    private Rigidbody2D rb2d;       //stores a reference to the Rigidbody2D component required to use the 2D physics engine
    private int count;              //variable to store the number of collectibles collected
    private int healthcount;        //variable to store the number of health points remaining


    // Use this for initialization
    void Start()
    {
        //get and store a reference to the Rigidbody2D component so you can access it
        rb2d = GetComponent<Rigidbody2D>();

        //initialize the collectibles counter to 0
        count = 0;

        //initialize health points to 3
        health = 3;

        //initializes null-string victory and defeat texts
        winText.text = "";
        loseText.text = "";

        //call the SetCountText function, which will update the text with the exact value depending on the collectibles collected 
        SetCountText();
    }

    //FixedUpdate is called at a fixed interval and is independent of frame rate. Put physics code here.
    void FixedUpdate()
    {
        //stores the current horizontal input in the float variable moveHorizontal
        float moveHorizontal = Input.GetAxis("Horizontal");

        //stores the current vertical input in the float variable moveVertical
        float moveVertical = Input.GetAxis("Vertical");

        //use the two stored floats to create a new motion variable Vector2
        Vector2 movement = new Vector2(moveHorizontal, moveVertical);

        //call the AddForce function of the Rigidbody2D component that will move the character by an amount given by motion * velocity
        rb2d.AddForce(movement * speed);
    }

    //the OnTriggerEnter2D function is called whenever the argument's collision box collides with another collision box scripted to be a trigger
    void OnTriggerEnter2D(Collider2D other)
    {
        //control blocks to see the object with which the collision occurred

        //if the object is tagged “PickUp”...
        if (other.gameObject.CompareTag("Coin"))
        {
            //... then set the latter to inactive.
            other.gameObject.SetActive(false);

            //after the object is collected, the sound of a collected coin starts
            coinDing.clip = ding;
            coinDing.Play();

            //increases the collectibles counter by one unit
            count = count + 1;

            //update the text
            SetCountText();
        }
        //if the object is tagged “Spikes”...
        else if (other.gameObject.CompareTag("Spikes"))
        {
            //...then call the TakeDamage() function and take an amount of damage in health points equal to 1
            TakeDamage(1);

            //if health drops to 0
            if (health <= 0)
            {
                //set the character to inactive
                player.SetActive(false);

                //update the text
                SetCountText();

                //call the death function
                Die();
            }
            
        }

    }

    //this function updates the text by showing the various messages in the UI
    void SetCountText()
    {
        countText.text = "Count: " + count.ToString();                  //show the number of collectibles
        healthText.text = "Health Points: " + health.ToString();        //shows the number of health points

        //check if we have collected all 6 coins
        if (count == 6)

            //if positive, it shows a message of victory
            winText.text = "You win!";

        //but if the health points drop to 0...
        else if (!player.activeSelf)
        {
            //...then it shows a message of defeat
            winText.text = "You lose!";
        }
    }

    //function that decreases health points on the amount of damage
    public void TakeDamage(float dmg)
    {
        health -= dmg;                                      //decreases health points
        healthText.text = "Health Points: " + health;       //update text
    }

    //function that is activated when the character is deactivated
    public void Die()
    {
        Application.LoadLevel(Application.loadedLevel);     //the level is reloaded from the beginning
    }

}