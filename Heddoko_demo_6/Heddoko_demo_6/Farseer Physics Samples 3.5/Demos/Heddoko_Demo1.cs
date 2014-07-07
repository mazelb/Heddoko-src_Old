using System.Text;
using System.IO.Ports;
using FarseerPhysics.Dynamics;
using FarseerPhysics.Factories;
using FarseerPhysics.Samples.Demos.Prefabs;
using FarseerPhysics.Samples.DrawingSystem;
using FarseerPhysics.Samples.ScreenSystem;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System;
using System.Timers;

namespace FarseerPhysics.Samples.Demos
{
    internal class Heddoko_Demo1 : PhysicsGameScreen, IDemoScreen
    {
        private Body _rectangleBase;
        private Sprite _rectangleBaseSprite;
        private Body _rectangle;
        private Sprite _rectangleSprite;
        private float _rotationAngle;

        #region Arduino communications 
        
        SerialPort _port;
        private string _lastData = "";

        /// <summary>
        /// StartConnectionToArduino.
        /// </summary>
        private void StartConnectionToArduino()
        {
            //  Signal the Arduino Board to start sending data
            // Set up the serial port communication with the Arduino on COM4 at 9600 baud
            _port = new SerialPort("COM3") { BaudRate = 9600 };

            //  hook up the event for receiving the data
            _port.DataReceived += _port_DataReceived;

            //open the port communication
            _port.Open();

        }

        /// <summary>
        /// EndConnectionToArduino.
        /// </summary>
        private void EndConnectionToArduino()
        {
            //  Signal the Arduino Board to stop sending data
            //
        }

        /// <summary>
        /// EndConnectionToArduino.
        /// </summary>
        private void _port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string data = _port.ReadExisting();
            _lastData += data;
            string[] allData = _lastData.Split('\n');
            float angleFiltered = 0f;
            //if (allData.Length > 12)
            if (allData.Length > 2)
            {
                //Averaging over the last 10 values
                //for (int i = 0; i < 10; i++ )
                {
                    //Get the angle of the elbow
                    //_lastData = allData[allData.Length-2-i];
                    _lastData = allData[allData.Length-2];
                    _lastData = _lastData.Trim();
                    angleFiltered += Convert.ToSingle(_lastData);
                }

                //_rotationAngle = MathHelper.Pi * (angleFiltered) / 1000.0f;
                _rotationAngle = MathHelper.Pi * (angleFiltered) / 100.0f;
            }
        }

        //private void _port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        //{
        //    string data = _port.ReadExisting();
        //    _lastData += data;
        //    string[] allData = _lastData.Split('\n');
        //    if (allData.Length > 2)
        //    {
        //        _lastData = allData[allData.Length - 2];
        //        _lastData = _lastData.Trim();
        //        _rotationAngle = MathHelper.Pi * Convert.ToSingle(_lastData) / 100.0f;
        //    }
        //}


        // Specify what you want to happen when the Elapsed event is raised.
        private static void _port_DataReceived_timer(object source, ElapsedEventArgs e)
        {
            Console.WriteLine("Arduino data simulation!");
        }

        #endregion

        #region IDemoScreen Members

        public string GetTitle()
        {
            return "Body with a single fixture";
        }

        public string GetDetails()
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendLine("This demo shows a single body with one attached fixture and shape.");
            sb.AppendLine("A fixture binds a shape to a body and adds material");
            sb.AppendLine("properties such as density, friction, and restitution.");
            sb.AppendLine(string.Empty);
            sb.AppendLine("GamePad:");
            sb.AppendLine("  - Rotate object: left and right triggers");
            sb.AppendLine("  - Move object: right thumbstick");
            sb.AppendLine("  - Move cursor: left thumbstick");
            sb.AppendLine("  - Grab object (beneath cursor): A button");
            sb.AppendLine("  - Drag grabbed object: left thumbstick");
            sb.AppendLine("  - Exit to menu: Back button");
            sb.AppendLine(string.Empty);
            sb.AppendLine("Keyboard:");
            sb.AppendLine("  - Rotate Object: left and right arrows");
            sb.AppendLine("  - Move Object: A,S,D,W");
            sb.AppendLine("  - Exit to menu: Escape");
            sb.AppendLine(string.Empty);
            sb.AppendLine("Mouse / Touchscreen");
            sb.AppendLine("  - Grab object (beneath cursor): Left click");
            sb.AppendLine("  - Drag grabbed object: move mouse / finger");
            return sb.ToString();
        }

        #endregion

        public override void LoadContent()
        {
            base.LoadContent();

            World.Gravity = Vector2.Zero;

            Texture2D armTexture = ScreenManager.Content.Load<Texture2D>("Arm/robot_arm_arm");
            Texture2D armBaseTexture = ScreenManager.Content.Load<Texture2D>("Arm/robot_arm_base");

            _rectangleBase = BodyFactory.CreateRectangle(World, 5f, 5f, 1f);
            _rectangleBase.BodyType = BodyType.Static;

            _rectangle = BodyFactory.CreateRectangle(World, 5f, 5f, 1f);
            _rectangle.BodyType = BodyType.Dynamic;

            SetUserAgent(_rectangle, 100f, 100f);

            // create sprite based on body
            _rectangleSprite = new Sprite(armTexture, new Vector2(1480, 1167));//ScreenManager.Assets.TextureFromShape(_rectangle.FixtureList[0].Shape, MaterialType.arm, Color.Orange, 1f));
            _rectangleBaseSprite = new Sprite(armBaseTexture);//ScreenManager.Assets.TextureFromShape(_rectangle.FixtureList[0].Shape, MaterialType.arm, Color.Orange, 1f));
            
            //Connect to arduino and start listening 
            StartConnectionToArduino();
        }

        public override void Draw(GameTime gameTime)
        {
            ScreenManager.SpriteBatch.Begin(0, null, null, null, null, null, Camera.View);
            ScreenManager.SpriteBatch.Draw(_rectangleBaseSprite.Texture, ConvertUnits.ToDisplayUnits(_rectangleBase.Position), null, Color.White, _rectangleBase.Rotation, _rectangleBaseSprite.Origin, 1f, SpriteEffects.None, 0f);
            ScreenManager.SpriteBatch.Draw(_rectangleSprite.Texture, ConvertUnits.ToDisplayUnits(_rectangle.Position), null, Color.White, _rectangle.Rotation, _rectangleSprite.Origin, 1f, SpriteEffects.None, 0f);
            ScreenManager.SpriteBatch.End();
            base.Draw(gameTime);
        }

        public override void Update(GameTime gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
        {
            // The time since Update was called last.
            float elapsed = (float)gameTime.ElapsedGameTime.TotalSeconds;

            _rectangle.Rotation = _rotationAngle;
            _rectangle.Position = new Vector2(19f, 6f);

            base.Update(gameTime, otherScreenHasFocus, coveredByOtherScreen);            
        }
    }
}