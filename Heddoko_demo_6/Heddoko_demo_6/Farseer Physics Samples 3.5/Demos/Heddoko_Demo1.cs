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

        public float MapValue(float value, float fromSource, float toSource, float fromTarget, float toTarget)
        {
            return (value - fromSource) / (toSource - fromSource) * (toTarget - fromTarget) + fromTarget;
        }

        /// <summary>
        /// EndConnectionToArduino.
        /// </summary>
        private void _port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            //string data = _port.ReadExisting();
            //_lastData += data;
            //string[] allData = _lastData.Split('\n');
            //if (allData.Length > 2)
            //{
            //    _lastData = allData[allData.Length - 2];
            //    _lastData = _lastData.Trim();
            //    _rotationAngle = MathHelper.Pi * Convert.ToSingle(_lastData) / 100.0f;
            //}

            string data = _port.ReadExisting();
            _lastData += data;
            string[] allData = _lastData.Split('\n');
            float angleFiltered = 0f;
            if (allData.Length > 7)
            {
                //Averaging over the last 10 values
                for (int i = 0; i < 5; i++ )
                {
                    //Get the angle of the elbow
                    //_lastData = allData[allData.Length-2-i];
                    _lastData = allData[allData.Length-2-i];
                    _lastData = _lastData.Trim();
                    angleFiltered += Convert.ToSingle(_lastData);
                }

                _rotationAngle = MapValue(angleFiltered / 5f, 0f, 100f, 0f, (3f * MathHelper.Pi / 3f));
                //_rotationAngle = MathHelper.Pi * (angleFiltered) / 500.0f;
            }
        }


        // Specify what you want to happen when the Elapsed event is raised.
        private static void _port_DataReceived_timer(object source, ElapsedEventArgs e)
        {
            Console.WriteLine("Data simulation!");
        }

        #endregion

        #region IDemoScreen Members

        public string GetTitle()
        {
            return "Heddoko Robot Arm";
        }

        public string GetDetails()
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendLine("This demo shows detection of an elbow joint movement.");
            sb.AppendLine("Bend arm with the compression sleeve to see effects");
            sb.AppendLine(string.Empty);
            sb.AppendLine("Keyboard:");
            sb.AppendLine("  - Exit to menu: Escape");
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